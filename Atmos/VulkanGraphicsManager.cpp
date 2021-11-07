#include "VulkanGraphicsManager.h"

#include "VulkanSurfaceResource.h"
#include "VulkanImageAssetResource.h"
#include "VulkanShaderAssetResource.h"
#include "VulkanTextResource.h"
#include "VulkanMemory.h"

#include "VulkanImage.h"
#include "VulkanCreateImageView.h"
#include "VulkanSwapchainSupport.h"
#include "VulkanStoredImageResource.h"

#include "VulkanSynchronization.h"
#include "VulkanCommandBuffer.h"
#include "VulkanResults.h"

#include "UpdateText.h"
#include "MainSurface.h"
#include "GraphicsError.h"

//DELETEME
#include "CreateStopwatch.h"

#include <Arca/Reliquary.h>

namespace Atmos::Render::Vulkan
{
    GraphicsManager::GraphicsManager(Logging::Logger& logger) :
        Render::GraphicsManager(logger, "Vulkan"),
        shaderCompiler(logger)
    {
#ifndef NDEBUG
        instanceLayers.push_back(VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME);
        instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

        ValidateRequiredInstanceExtensions();
        ValidateRequiredInstanceLayers();

        instance = CreateInstance();
        debug = std::make_unique<Debug>(instance);
        Debug::logger = &logger;
    }

    GraphicsManager::~GraphicsManager()
    {
        WaitForSurfacesIdle();
    }
    
    void GraphicsManager::SetFullscreen(bool set)
    {

    }

    void GraphicsManager::ChangeVerticalSync(bool set)
    {
        
    }

    std::unique_ptr<Asset::Resource::Image> GraphicsManager::CreateImageResourceImpl(
        const Bytes& bytes,
        const Name& name,
        const Spatial::Size2D& size)
    {
        auto imageData = CreateImageData({ {bytes, size} });
        return std::make_unique<Asset::Resource::Vulkan::Image>(imageData[0]);
    }

    std::unique_ptr<Asset::Resource::Shader> GraphicsManager::CreateShaderResourceImpl(
        const Bytes& bytes, const Name& name)
    {
        const vk::ShaderModuleCreateInfo createInfo(
            {},
            bytes.size(),
            reinterpret_cast<const uint32_t*>(bytes.data()));
        auto created = device.createShaderModuleUnique(createInfo);
        return std::make_unique<Asset::Resource::Vulkan::Shader>(std::move(created));
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateMainSurfaceResourceImpl(
        void* window)
    {
        auto surface = CreateSurface(window, instance);

        const auto pickedPhysicalDevice = PickPhysicalDevice(surface.get(), instance, deviceExtensions);
        if (!pickedPhysicalDevice)
            throw GraphicsError("Could not pick a physical device.");
        physicalDevice = *pickedPhysicalDevice;
        memoryProperties = physicalDevice.getMemoryProperties();

        const auto queueIndices = SuitableQueueFamilies(physicalDevice, surface.get());
        if (!queueIndices)
            throw GraphicsError("Could not create surface.");

        device = CreateDevice(physicalDevice, *queueIndices, deviceExtensions, instanceLayers);
        memoryPool = MemoryPool(2048000, memoryProperties, device);

        sampler = CreateSampler(device);

        graphicsQueue = device.getQueue(queueIndices->graphicsFamily, 0);
        const auto presentQueue = device.getQueue(queueIndices->presentFamily, 0);

        commandPool = CreateCommandPool(device, *queueIndices);
        auto commandBuffers = CreateCommandBuffers(device, commandPool.get(), vk::CommandBufferLevel::ePrimary, 1);
        imageCommandBuffer = std::move(commandBuffers[0]);
        imageFence = CreateFence(device, {});

        return CreateSurfaceResourceCommon(std::move(surface), *queueIndices, graphicsQueue, presentQueue);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateSurfaceResourceImpl(
        void* window)
    {
        auto surface = CreateSurface(window, instance);
        const auto queueIndices = SuitableQueueFamilies(physicalDevice, surface.get());
        if (!queueIndices)
            throw GraphicsError("Could not create surface.");
        const auto presentQueue = device.getQueue(queueIndices->presentFamily, 0);
        return CreateSurfaceResourceCommon(std::move(surface), *queueIndices, graphicsQueue, presentQueue);
    }

    std::unique_ptr<Resource::Text> GraphicsManager::CreateTextResourceImpl(
        const Atmos::Buffer& buffer, const Spatial::Size2D& size)
    {
        const auto imageData = CreateImageData({ {buffer, size} });
        return std::make_unique<Resource::Vulkan::Text>(imageData[0]);
    }

    void GraphicsManager::StageImpl(const RenderImage& render)
    {
        StageRender(render);
    }

    void GraphicsManager::StageImpl(const RenderLine& render)
    {
        StageRender(render);
    }

    void GraphicsManager::StageImpl(const RenderRegion& render)
    {
        StageRender(render);
    }

    void GraphicsManager::StageImpl(const RenderText& render)
    {
        StageRender(render);
    }

    void GraphicsManager::StageImpl(const UpdateText& update)
    {
        const auto found = textUpdates.find(update.text);
        if (found == textUpdates.end())
            textUpdates.emplace(update.text, update);
        else
            found->second = update;
    }

    void GraphicsManager::DrawFrameImpl(Resource::Surface& surface, const Spatial::Point2D& mapPosition, const Color& backgroundColor, Diagnostics::Statistics::Profile& profile)
    {
        auto stopwatch = Time::CreateRealStopwatch();
        UpdateTexts();

        profile.NewTime(stopwatch);

        PruneResourcesImpl();

        const auto& vulkan = RequiredResource<Resource::Vulkan::Surface>(surface);
        vulkan.backing->DrawFrame(mapPosition, backgroundColor);
    }

    void GraphicsManager::ResourceDestroyingImpl(Asset::Resource::Image& resource)
    {
        const auto& image = RequiredResource<Asset::Resource::Vulkan::Image>(resource);
        MoveToDestroyedResource(*image.imageData.storedResource);
    }

    void GraphicsManager::ResourceDestroyingImpl(Asset::Resource::Shader& resource)
    {}

    void GraphicsManager::ResourceDestroyingImpl(Resource::Surface& resource)
    {
        const auto& vulkan = RequiredResource<Resource::Vulkan::Surface>(resource);
        for (auto surface = surfaces.begin(); surface != surfaces.end(); ++surface)
        {
            if (surface->get() == vulkan.backing)
            {
                surfaces.erase(surface);
                return;
            }
        }
    }

    void GraphicsManager::ResourceDestroyingImpl(Resource::Text& resource)
    {
        const auto& vulkan = RequiredResource<Resource::Vulkan::Text>(resource);
        MoveToDestroyedResource(*vulkan.imageData.storedResource);
    }

    void GraphicsManager::PruneResourcesImpl()
    {
        WaitForSurfacesIdle();
        destroyedResources.clear();
    }

    Atmos::Buffer GraphicsManager::CompileShaderImpl(const File::Path& filePath)
    {
#ifndef NDEBUG
        return shaderCompiler.CompileWithDebugging(filePath);
#else
        return shaderCompiler.Compile(filePath);
#endif
    }

    bool GraphicsManager::ShouldReconstructInternals() const
    {
        return false;
    }

    void GraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
    {
        const auto& resource = *objects.mainSurface->Resource<Resource::Vulkan::Surface>();
        const auto queueIndices = SuitableQueueFamilies(physicalDevice, resource.backing->Underlying());
        resource.backing->Reinitialize(*queueIndices);
    }
    
    void GraphicsManager::MoveToDestroyedResource(StoredResource& resource)
    {
        for(auto storedResource = storedResources.begin(); storedResource != storedResources.end(); ++storedResource)
        {
            if (storedResource->get() == &resource)
            {
                destroyedResources.push_back(std::move(*storedResource));
                storedResources.erase(storedResource);
                return;
            }
        }
    }
    
    vk::Instance GraphicsManager::CreateInstance()
    {
        const vk::ApplicationInfo applicationInfo({}, {}, {}, {}, VK_API_VERSION_1_2);

        vk::InstanceCreateInfo createInfo(
            {},
            &applicationInfo,
            {},
            {},
            static_cast<uint32_t>(instanceExtensions.size()),
            instanceExtensions.data());
        if (!instanceLayers.empty())
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size());
            createInfo.ppEnabledLayerNames = instanceLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }
        const auto debugCreateInfo = debug->CreateInfo();
        createInfo.pNext = &debugCreateInfo;

        return vk::createInstance(createInfo);
    }

    void GraphicsManager::ValidateRequiredInstanceExtensions() const
    {
        const auto available = vk::enumerateInstanceExtensionProperties();
        std::set<String> availableNames;
        for (auto& properties : available)
            availableNames.emplace(ToString(properties.extensionName));

        const auto unavailableSet = Unavailable(instanceExtensions, availableNames);
        if (!unavailableSet.empty())
            throw GraphicsError("Not all instance extensions are available.",
                { { "Unavailable", Chroma::Join(", ", unavailableSet.begin(), unavailableSet.end())} });
    }

    void GraphicsManager::ValidateRequiredInstanceLayers() const
    {
        const auto available = vk::enumerateInstanceLayerProperties();
        std::set<String> availableNames;
        for (auto& properties : available)
            availableNames.emplace(ToString(properties.layerName));

        const auto unavailableSet = Unavailable(instanceLayers, availableNames);
        if (!unavailableSet.empty())
            throw GraphicsError("Not all instance layers are available.",
                { { "Unavailable", Chroma::Join(", ", unavailableSet.begin(), unavailableSet.end())} });
    }

    std::optional<vk::PhysicalDevice> GraphicsManager::PickPhysicalDevice(
        vk::SurfaceKHR mainSurface, vk::Instance instance, const std::vector<const char*>& deviceExtensions)
    {
        const auto availablePhysicalDevices = instance.enumeratePhysicalDevices();
        if (availablePhysicalDevices.empty())
            throw GraphicsError("No physical devices found.");

        for (const auto& checkDevice : availablePhysicalDevices)
        {
            const auto suitableQueueFamilies = SuitableQueueFamilies(checkDevice, mainSurface);
            if (!suitableQueueFamilies)
                continue;

            if (!HasRequiredDeviceExtensionProperties(checkDevice, deviceExtensions))
                continue;

            const auto swapChainSupportDetails = SwapchainSupport(checkDevice, mainSurface);
            if (swapChainSupportDetails.formats.empty() || swapChainSupportDetails.presentModes.empty())
                continue;

            auto supportedFeatures = checkDevice.getFeatures();
            if (!supportedFeatures.samplerAnisotropy)
                continue;

            return checkDevice;
        }

        return {};
    }

    bool GraphicsManager::HasRequiredDeviceExtensionProperties(
        vk::PhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions)
    {
        const auto available = physicalDevice.enumerateDeviceExtensionProperties();
        std::set<String> availableNames;
        for (auto& properties : available)
            availableNames.emplace(ToString(properties.extensionName));

        const auto unavailableSet = Unavailable(deviceExtensions, availableNames);
        return unavailableSet.empty();
    }

    vk::Device GraphicsManager::CreateDevice(
        vk::PhysicalDevice physicalDevice,
        QueueFamilyIndices queueFamilyIndices,
        const std::vector<const char*>& deviceExtensions,
        const std::vector<const char*>& instanceLayers)
    {
        const std::set uniqueQueueFamilies =
        {
            queueFamilyIndices.graphicsFamily,
            queueFamilyIndices.presentFamily
        };
        constexpr auto queuePriority = 1.0f;

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(uniqueQueueFamilies.size());
        for (const auto queueFamily : uniqueQueueFamilies)
            queueCreateInfos.push_back(vk::DeviceQueueCreateInfo({}, queueFamily, 1, &queuePriority));

        vk::PhysicalDeviceFeatures physicalDeviceFeatures;
        physicalDeviceFeatures.wideLines = true;
        physicalDeviceFeatures.samplerAnisotropy = true;

        vk::DeviceCreateInfo createInfo(
            {},
            queueCreateInfos.size(),
            queueCreateInfos.data(),
            {},
            {},
            static_cast<uint32_t>(deviceExtensions.size()),
            deviceExtensions.data(),
            &physicalDeviceFeatures);
        if (!instanceLayers.empty())
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size());
            createInfo.ppEnabledLayerNames = instanceLayers.data();
        }
        else
            createInfo.enabledLayerCount = 0;

        return vk::Device(physicalDevice.createDevice(createInfo));
    }

    vk::UniqueCommandPool GraphicsManager::CreateCommandPool(vk::Device device, QueueFamilyIndices queueFamilyIndices)
    {
        const vk::CommandPoolCreateInfo createInfo(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueFamilyIndices.graphicsFamily);
        return device.createCommandPoolUnique(createInfo);
    }

    auto GraphicsManager::SuitableQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
        -> std::optional<QueueFamilyIndices>
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        const auto availableQueueFamilyProperties = physicalDevice.getQueueFamilyProperties();
        uint32_t i = 0;
        for (auto& properties : availableQueueFamilyProperties)
        {
            if (properties.queueFlags & vk::QueueFlagBits::eGraphics)
                graphicsFamily = i;

            if (physicalDevice.getSurfaceSupportKHR(i, surface))
                presentFamily = i;

            ++i;
        }

        return graphicsFamily && presentFamily
            ? QueueFamilyIndices{ graphicsFamily.value(), presentFamily.value() }
            : QueueFamilyIndices{};
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateSurfaceResourceCommon(
        vk::UniqueSurfaceKHR&& underlying,
        QueueFamilyIndices queueIndices,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue)
    {
        surfaces.push_back(std::make_unique<Resource::Vulkan::BackingSurface>(
            device,
            physicalDevice,
            std::move(underlying),
            sampler.get(),
            graphicsQueue,
            presentQueue,
            queueIndices,
            memoryProperties,
            Logger()));
        
        return std::make_unique<Resource::Vulkan::Surface>(*surfaces.back());
    }

    vk::UniqueSurfaceKHR GraphicsManager::CreateSurface(void* window, vk::Instance instance)
    {
        const vk::Win32SurfaceCreateInfoKHR createInfo({}, GetModuleHandle(nullptr), HWND(window));
        return instance.createWin32SurfaceKHRUnique(createInfo);
    }

    void GraphicsManager::WaitForSurfacesIdle()
    {
        for (const auto& surface : surfaces)
            surface->WaitForIdle();
    }

    vk::UniqueSampler GraphicsManager::CreateSampler(vk::Device device)
    {
        constexpr vk::SamplerCreateInfo createInfo(
            {},
            vk::Filter::eNearest,
            vk::Filter::eNearest,
            vk::SamplerMipmapMode::eNearest,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            true,
            16,
            false,
            vk::CompareOp::eAlways,
            0.0f,
            0.0f,
            vk::BorderColor::eIntOpaqueBlack,
            false);

        return device.createSamplerUnique(createInfo);
    }

    void GraphicsManager::UpdateTexts()
    {
        std::vector<ImageDataPrototype> prototypes;
        std::vector<Resource::Vulkan::Text*> texts;
        for (const auto& update : textUpdates)
        {
            prototypes.emplace_back(std::move(update.second.buffer), update.second.size);
            texts.push_back(&RequiredResource<Resource::Vulkan::Text>(*update.second.text));
        }

        const auto imageDatas = CreateImageData(prototypes);

        for(size_t i = 0; i < prototypes.size(); ++i)
        {
            auto& text = *texts[i];
            MoveToDestroyedResource(*text.imageData.storedResource);
            text.imageData = imageDatas[i];
        }

        textUpdates.clear();
    }

    std::vector<ImageData> GraphicsManager::CreateImageData(const std::vector<ImageDataPrototype>& prototypes)
    {
        struct Constructing
        {
            Spatial::Size2D size;
            vk::UniqueImage image;
            UniqueMemory memory;
            std::optional<Buffer> stagingBuffer;
            Atmos::Buffer bytes;
        };

        std::vector<Constructing> allConstructing;

        static constexpr auto format = vk::Format::eR8G8B8A8Srgb;
        static constexpr auto finalImageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        RecordAndSubmit(
            *imageCommandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            graphicsQueue,
            imageFence.get(),
            [this, &prototypes, &allConstructing](auto record)
            {
                for (auto& [buffer, size] : prototypes)
                {
                    const auto width = static_cast<uint32_t>(size.width);
                    const auto height = static_cast<uint32_t>(size.height);
                    const auto extent = vk::Extent3D(width, height, 1);

                    constexpr auto layerCount = 1;

                    auto image = device.createImageUnique(vk::ImageCreateInfo(
                        {},
                        vk::ImageType::e2D,
                        format,
                        extent,
                        1,
                        layerCount,
                        vk::SampleCountFlagBits::e1,
                        vk::ImageTiling::eOptimal,
                        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                        vk::SharingMode::eExclusive,
                        {},
                        {},
                        vk::ImageLayout::eUndefined));
                    
                    auto memory = memoryPool->Bind(image.get(), vk::MemoryPropertyFlagBits::eDeviceLocal);

                    record(TransitionLayout(*image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, 0, 1));

                    allConstructing.push_back(Constructing{ size, std::move(image), std::move(memory), {}, std::move(buffer) });
                }
            });
        if (IsError(WaitAndReset(device, imageFence.get())))
            Logger().Log("Could not wait for Vulkan fences.");

        for (auto& constructing : allConstructing)
        {
            const auto& bytes = constructing.bytes;
            Buffer stagingBuffer(
                vk::DeviceSize(bytes.size()),
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                device,
                *memoryPool);
            stagingBuffer.PushBytes(bytes, 0);
            constructing.stagingBuffer = std::move(stagingBuffer);
        }

        RecordAndSubmit(
            *imageCommandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            graphicsQueue,
            imageFence.get(),
            [this, &allConstructing](auto record)
            {
                for (auto& [size, image, memory, stagingBuffer, bytes] : allConstructing)
                {
                    const auto width = static_cast<uint32_t>(size.width);
                    const auto height = static_cast<uint32_t>(size.height);
                    record(stagingBuffer->Copy(
                        image.get(),
                        0,
                        0,
                        0,
                        { 0, 0, 0 },
                        { width, height, 1 },
                        0,
                        1));
                    record(TransitionLayout(*image, vk::ImageLayout::eTransferDstOptimal, finalImageLayout, 0, 1));
                }
            });
        if (IsError(WaitAndReset(device, imageFence.get())))
            Logger().Log("Could not wait for Vulkan fences.");

        std::vector<ImageData> imageDatas;

        for (auto& [size, image, memory, stagingBuffer, bytes] : allConstructing)
        {
            auto imageView = CreateImageView(image.get(), device, format, 0, 1);

            const auto descriptor = CombinedImageSamplerDescriptor(
                imageView.get(), sampler.get(), finalImageLayout, 1);

            auto storedResource = std::make_unique<StoredImageResource>(
                std::move(memory), std::move(imageView), std::move(image));
            const auto imageData = ImageData{
                size,
                storedResource->image.get(),
                storedResource->imageView.get(),
                finalImageLayout,
                descriptor,
                *storedResource };

            imageDatas.push_back(imageData);
            storedResources.push_back(std::move(storedResource));
        }

        return imageDatas;
    }
}
