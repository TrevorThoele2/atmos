#include "VulkanGraphicsManager.h"

#include "VulkanSurfaceResource.h"
#include "VulkanImageAssetResource.h"
#include "VulkanShaderAssetResource.h"

#include "VulkanImage.h"
#include "VulkanCreateImageView.h"
#include "VulkanSwapchainSupport.h"

#include "MainSurface.h"
#include "GraphicsError.h"

#include <Arca/Reliquary.h>

namespace Atmos::Render::Vulkan
{
    GraphicsManager::GraphicsManager(Logging::Logger& logger) :
        Render::GraphicsManager(logger), shaderCompiler(logger)
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

    bool GraphicsManager::IsOk() const
    {
        return true;
    }

    void GraphicsManager::SetFullscreen(bool set)
    {

    }

    void GraphicsManager::ChangeVerticalSync(bool set)
    {
        
    }

    String GraphicsManager::TypeName() const
    {
        return "Vulkan";
    }

    std::unique_ptr<Asset::Resource::Image> GraphicsManager::CreateImageResourceImpl(
        const Bytes& bytes,
        const Name& name,
        const Asset::ImageSize& size)
    {
        const auto width = static_cast<uint32_t>(size.width);
        const auto height = static_cast<uint32_t>(size.height);
        const auto extent = vk::Extent3D(width, height, 1);

        const auto format = vk::Format::eR8G8B8A8Srgb;

        auto image = Image(
            format,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
            extent,
            1,
            vk::ImageLayout::eUndefined,
            *device,
            memoryProperties);
        image.TransitionLayout(vk::ImageLayout::eTransferDstOptimal, 0, 1, *device, *commandPool, graphicsQueue);

        Buffer stagingBuffer(
            vk::DeviceSize(bytes.size()),
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            *device,
            memoryProperties);
        stagingBuffer.PushBytes(bytes, 0);

        stagingBuffer.Copy(
            image.value.get(),
            0,
            0,
            0,
            { 0, 0, 0 },
            { width, height, 1 },
            0,
            1,
            *commandPool,
            graphicsQueue);
        image.TransitionLayout(vk::ImageLayout::eShaderReadOnlyOptimal, 0, 1, *device, *commandPool, graphicsQueue);

        auto imageView = CreateImageView(image.value.get(), *device, format, 0, 1);

        auto descriptor = CombinedImageSamplerDescriptor(
            imageView.get(), sampler.get(), vk::ImageLayout::eShaderReadOnlyOptimal, 1);

        return std::make_unique<Asset::Resource::Vulkan::Image>(
            size, image.value.release(), image.memory.release(), imageView.release(), descriptor);
    }

    std::unique_ptr<Asset::Resource::Shader> GraphicsManager::CreateShaderResourceImpl(
        const Bytes& bytes, const Name& name)
    {
        const vk::ShaderModuleCreateInfo createInfo(
            {},
            bytes.size(),
            reinterpret_cast<const uint32_t*>(bytes.data()));
        auto created = device->createShaderModule(createInfo);
        return std::make_unique<Asset::Resource::Vulkan::Shader>(created, device);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateMainSurfaceResourceImpl(
        void* window,
        Arca::Reliquary& reliquary)
    {
        auto surface = CreateSurface(window, instance);

        auto pickedPhysicalDevice = PickPhysicalDevice(surface.get(), instance, deviceExtensions);
        if (!pickedPhysicalDevice)
            throw GraphicsError("Could not pick a physical device.");
        physicalDevice = *pickedPhysicalDevice;
        memoryProperties = physicalDevice.getMemoryProperties();

        const auto queueIndices = SuitableQueueFamilies(physicalDevice, surface.get());
        if (!queueIndices)
            throw GraphicsError("Could not create surface.");

        device = CreateDevice(instance, physicalDevice, *queueIndices, deviceExtensions, instanceLayers);

        sampler = CreateSampler(*device);

        graphicsQueue = device->getQueue(queueIndices->graphicsFamily, 0);
        const auto presentQueue = device->getQueue(queueIndices->presentFamily, 0);

        commandPool = CreateCommandPool(*device, *queueIndices);

        return CreateSurfaceResourceCommon(std::move(surface), *queueIndices, graphicsQueue, presentQueue, reliquary);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateSurfaceResourceImpl(
        void* window,
        Arca::Reliquary& reliquary)
    {
        auto surface = CreateSurface(window, instance);
        const auto queueIndices = SuitableQueueFamilies(physicalDevice, surface.get());
        if (!queueIndices)
            throw GraphicsError("Could not create surface.");
        const auto presentQueue = device->getQueue(queueIndices->presentFamily, 0);
        return CreateSurfaceResourceCommon(std::move(surface), *queueIndices, graphicsQueue, presentQueue, reliquary);
    }

    void GraphicsManager::ResourceDestroyingImpl(Asset::Resource::Image& resource)
    {
        auto& casted = static_cast<Asset::Resource::Vulkan::Image&>(resource);
        storedResourceList.push_back(std::make_unique<StoredImageResource>(
            casted.image, casted.memory, casted.imageView, device));
    }

    void GraphicsManager::PruneResourcesImpl(Arca::Reliquary& reliquary)
    {
        auto surfaces = reliquary.Batch<SurfaceCore>();
        for(auto& surface : surfaces)
        {
            auto& casted = static_cast<Resource::Vulkan::Surface&>(*surface.resource);
            casted.WaitForIdle();
        }
        storedResourceList.clear();
    }

    File::Path GraphicsManager::CompileShaderImpl(
        const File::Path& inputFilePath, const std::optional<File::Path>& outputFilePath)
    {
        const auto useInputFilePath = inputFilePath;
        File::Path useOutputFilePath;
        if (outputFilePath)
            useOutputFilePath = *outputFilePath;
        else
        {
            useOutputFilePath = useInputFilePath;

            const auto extension = Chroma::ReplaceString(
                Chroma::ToString(useOutputFilePath.extension()),
                ".",
                "");
            const auto oldFileName = Chroma::ToString(useOutputFilePath.filename());
            const auto newFileName = Chroma::ReplaceString(
                oldFileName,
                String(".") + extension,
                String("_") + extension + ".spv");

            useOutputFilePath.replace_filename(newFileName);
        }

#ifndef NDEBUG
        shaderCompiler.CompileWithDebugging(useInputFilePath, useOutputFilePath);
#else
        shaderCompiler.Compile(useInputFilePath, useOutputFilePath);
#endif

        return useOutputFilePath;
    }

    bool GraphicsManager::ShouldReconstructInternals() const
    {
        return false;
    }

    void GraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
    {
        const auto handleSurface = [&](Resource::Vulkan::Surface& resource)
        {
            const auto queueIndices = SuitableQueueFamilies(physicalDevice, resource.Underlying());
            resource.Reinitialize(*queueIndices);
        };

        handleSurface(*objects.mainSurface->Resource<Resource::Vulkan::Surface>());
    }

    GraphicsManager::StoredResource::~StoredResource() = default;

    GraphicsManager::StoredImageResource::StoredImageResource(
        vk::Image image, vk::DeviceMemory memory, vk::ImageView imageView, std::shared_ptr<vk::Device> device)
        :
        image(image),
        memory(memory),
        imageView(imageView),
        device(device)
    {}

    GraphicsManager::StoredImageResource::~StoredImageResource()
    {
        device->freeMemory(memory);
        device->destroyImageView(imageView);
        device->destroyImage(image);
    }

    vk::Instance GraphicsManager::CreateInstance()
    {
        vk::ApplicationInfo applicationInfo({}, {}, {}, {}, VK_API_VERSION_1_2);

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
        auto debugCreateInfo = debug->CreateInfo();
        createInfo.pNext = &debugCreateInfo;

        return vk::createInstance(createInfo);
    }

    void GraphicsManager::ValidateRequiredInstanceExtensions() const
    {
        const auto available = vk::enumerateInstanceExtensionProperties();
        std::set<String> availableNames;
        for (auto& properties : available)
            availableNames.emplace(properties.extensionName);

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
            availableNames.emplace(properties.layerName);

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
            availableNames.emplace(properties.extensionName);

        const auto unavailableSet = Unavailable(deviceExtensions, availableNames);
        return unavailableSet.empty();
    }

    std::shared_ptr<vk::Device> GraphicsManager::CreateDevice(
        vk::Instance instance,
        vk::PhysicalDevice physicalDevice,
        QueueFamilyIndices queueFamilyIndices,
        const std::vector<const char*>& deviceExtensions,
        const std::vector<const char*>& instanceLayers)
    {
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies =
        {
            queueFamilyIndices.graphicsFamily,
            queueFamilyIndices.presentFamily
        };
        auto queuePriority = 1.0f;
        for (auto queueFamily : uniqueQueueFamilies)
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
        {
            createInfo.enabledLayerCount = 0;
        }

        const auto device = new vk::Device(physicalDevice.createDevice(createInfo));
        return std::shared_ptr<vk::Device>(
            device,
            [instance](vk::Device* device)
            {
                device->destroy();
                instance.destroy();
                delete device;
            });
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
        vk::Queue presentQueue,
        Arca::Reliquary& reliquary)
    {
        return std::make_unique<Resource::Vulkan::Surface>(
            device,
            physicalDevice,
            std::move(underlying),
            sampler.get(),
            graphicsQueue,
            presentQueue,
            queueIndices,
            memoryProperties,
            reliquary,
            Logger());
    }

    vk::UniqueSurfaceKHR GraphicsManager::CreateSurface(void* window, vk::Instance instance)
    {
        const vk::Win32SurfaceCreateInfoKHR createInfo({}, GetModuleHandle(nullptr), HWND(window));
        return instance.createWin32SurfaceKHRUnique(createInfo);
    }

    vk::UniqueSampler GraphicsManager::CreateSampler(vk::Device device)
    {
        const vk::SamplerCreateInfo createInfo(
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
}
