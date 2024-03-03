#include "VulkanGraphicsManager.h"

#include "VulkanSurfaceResource.h"
#include "VulkanImageAssetResource.h"
#include "VulkanShaderAssetResource.h"
#include "VulkanMemory.h"

#include "VulkanImage.h"
#include "VulkanSwapchainSupport.h"
#include "VulkanStoredImageResource.h"
#include "VulkanSampler.h"

#include "VulkanSynchronization.h"
#include "VulkanCommandBuffer.h"

#include "MainSurface.h"
#include "GraphicsError.h"

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
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        ValidateRequiredInstanceExtensions();
        ValidateRequiredInstanceLayers();

        instance = CreateInstance();
        try
        {
            debug = std::make_unique<Debug>(instance);
            Debug::logger = &logger;
        }
        catch (const GraphicsError& error)
        {
            logger.Log(
                "Could not initialize Vulkan debugging.",
                Logging::Severity::Verbose,
                Logging::Details
                {
                    { "Reason", error.Message() }
                });
        }
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
        auto imageData = CreateImageData(bytes, size);
        return std::make_unique<Asset::Resource::Vulkan::Image>(imageData);
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

        glyphAtlas = std::make_unique<GlyphAtlas>(
            device,
            imageCommandBuffer.get(),
            graphicsQueue,
            imageFence.get(),
            memoryProperties,
            Logger());

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
    
    void GraphicsManager::DrawFrameImpl(const AllRenders& allRenders, const Spatial::Point2D& mapPosition)
    {
        PruneResourcesImpl();

        std::unordered_map<Resource::Vulkan::Surface*, AllRenders> surfaces;

        const auto decomposeRenders = [&surfaces, &allRenders](auto rendersSelector)
        {
            for (auto& render : (allRenders.*rendersSelector))
            {
                auto& surface = RequiredResource<Resource::Vulkan::Surface>(*render.surface);
                auto found = surfaces.find(&surface);
                if (found == surfaces.end())
                    found = surfaces.emplace(&surface, AllRenders{}).first;

                (found->second.*rendersSelector).push_back(render);
            }
        };

        decomposeRenders(&AllRenders::images);
        decomposeRenders(&AllRenders::lines);
        decomposeRenders(&AllRenders::regions);
        decomposeRenders(&AllRenders::texts);

        for(auto& surface : surfaces)
            surface.first->backing->DrawFrame(surface.second, mapPosition);
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

    Spatial::Size2D GraphicsManager::TextBaseSizeImpl(
        const String& string, const Asset::Resource::Font& resource, bool bold, bool italics, float wrapWidth) const
    {
        const auto lines = glyphAtlas->ToLines({ &resource, bold, italics }, string, wrapWidth);
        return lines ? lines->totalSize : Spatial::Size2D{};
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
        const vk::ApplicationInfo applicationInfo(nullptr, 0, nullptr, 0, VK_API_VERSION_1_2);

        vk::InstanceCreateInfo createInfo(
            {},
            &applicationInfo,
            static_cast<uint32_t>(instanceLayers.size()),
            !instanceLayers.empty() ? instanceLayers.data() : nullptr,
            static_cast<uint32_t>(instanceExtensions.size()),
            !instanceExtensions.empty() ? instanceExtensions.data() : nullptr);

#ifndef NDEBUG
        const auto debugCreateInfo = Debug::CreateInfo();
        createInfo.pNext = &debugCreateInfo;
#endif

        vk::Instance instance;

        const auto result = vk::createInstance(&createInfo, nullptr, &instance);
        if (result == vk::Result::eErrorLayerNotPresent)
            throw GraphicsError("Layer not present.");
        else if (result == vk::Result::eErrorExtensionNotPresent)
            throw GraphicsError("Extension not present.");
        else if (result == vk::Result::eErrorOutOfHostMemory)
            throw GraphicsError("Out of host memory.");
        else if (result == vk::Result::eErrorOutOfDeviceMemory)
            throw GraphicsError("Out of device memory.");
        else if (result == vk::Result::eErrorInitializationFailed)
            throw GraphicsError("Initialization failed.");
        else if (result == vk::Result::eErrorIncompatibleDriver)
            throw GraphicsError("Incompatible driver.");
        else if (result != vk::Result::eSuccess)
            throw GraphicsError("Unknown Vulkan error.");

        return instance;
    }

    void GraphicsManager::ValidateRequiredInstanceExtensions() const
    {
        const auto available = vk::enumerateInstanceExtensionProperties();
        std::set<String> availableNames;
        for (auto& properties : available)
            availableNames.emplace(ToString(properties.extensionName));
        DoValidateRequired(availableNames, "instance extensions", instanceExtensions);
    }

    void GraphicsManager::ValidateRequiredInstanceLayers() const
    {
        const auto available = vk::enumerateInstanceLayerProperties();
        std::set<String> availableNames;
        for (auto& properties : available)
            availableNames.emplace(ToString(properties.layerName));
        DoValidateRequired(availableNames, "instance layers", instanceLayers);
    }

    void GraphicsManager::DoValidateRequired(
        const std::set<String>& availableNames, const String& descriptor, const std::vector<const char*>& requested) const
    {
        Logger().Log(
            "Vulkan " + descriptor + ".",
            Logging::Severity::Information,
            Logging::Details
            {
                { "Requested", Chroma::Join(", ", requested.begin(), requested.end()) },
                { "Available", Chroma::Join(", ", availableNames.begin(), availableNames.end())}
            });

        const auto unavailableSet = Unavailable(requested, availableNames);
        if (!unavailableSet.empty())
            throw GraphicsError("Not all " + descriptor + " are available.",
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
            *glyphAtlas,
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

    ImageData GraphicsManager::CreateImageData(const Atmos::Buffer& buffer, const Spatial::Size2D& size)
    {
        static constexpr auto finalImageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        auto image = CreateImage(
            device,
            static_cast<uint32_t>(size.width),
            static_cast<uint32_t>(size.height),
            defaultImageFormat,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
            InitialImageLayout::Undefined);

        auto memory = memoryPool->Bind(image.get(), vk::MemoryPropertyFlagBits::eDeviceLocal);

        Record(
            *imageCommandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            [&](auto record)
            {
                record(TransitionLayout(
                    *image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, 0, 1));
            });
        Submit(*imageCommandBuffer, graphicsQueue, imageFence.get());

        Buffer stagingBuffer(
            vk::DeviceSize(buffer.size()),
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            device,
            *memoryPool);
        stagingBuffer.PushBytes(buffer, 0);

        WaitAndReset(device, imageFence.get(), Logger());

        Record(
            *imageCommandBuffer,
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            [&](auto record)
            {
                const auto width = static_cast<uint32_t>(size.width);
                const auto height = static_cast<uint32_t>(size.height);
                record(stagingBuffer.Copy(
                    image.get(),
                    0,
                    0,
                    0,
                    { 0, 0, 0 },
                    { width, height, 1 },
                    0,
                    1));
                record(TransitionLayout(*image, vk::ImageLayout::eTransferDstOptimal, finalImageLayout, 0, 1));
            });

        Submit(*imageCommandBuffer, graphicsQueue, imageFence.get());

        auto imageView = CreateImageView(image.get(), device, defaultImageFormat, 0, 1);

        const auto descriptor = CombinedImageSamplerDescriptor(
            imageView.get(), sampler.get(), finalImageLayout, 1);

        auto storedResource = std::make_unique<StoredImageResource>(
            std::move(memory), std::move(imageView), std::move(image));

        WaitAndReset(device, imageFence.get(), Logger());

        const auto imageData = ImageData{
            size,
            storedResource->image.get(),
            storedResource->imageView.get(),
            finalImageLayout,
            descriptor,
            *storedResource };
        storedResources.push_back(std::move(storedResource));
        return imageData;
    }
}
