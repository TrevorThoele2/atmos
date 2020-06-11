#include "VulkanGraphicsManager.h"

#include "VulkanSurfaceData.h"
#include "VulkanImageAssetData.h"
#include "VulkanShaderAssetData.h"

#include "VulkanImage.h"
#include "VulkanCreateImageView.h"
#include "VulkanSwapchainSupport.h"

#include "AncillarySurface.h"
#include "MainSurface.h"
#include "GraphicsError.h"

#include "Log.h"

#include "WindowProvider.h"
#include "SimpleFile.h"

#include <Arca/Reliquary.h>

namespace Atmos::Render::Vulkan
{
    GraphicsManager::GraphicsManager()
    {
#ifndef NDEBUG
        instanceLayers.push_back(VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME);
        instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

        ValidateRequiredInstanceExtensions();
        ValidateRequiredInstanceLayers();

        instance = CreateInstance();
        debug.Initialize(instance);
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

    void GraphicsManager::SetupDefaultsImpl(Arca::Reliquary& reliquary)
    {
        
    }

    std::unique_ptr<Asset::ImageData> GraphicsManager::CreateImageDataImpl(
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

        return std::make_unique<ImageAssetDataImplementation>(
            std::move(image), std::move(imageView), descriptor);
    }

    std::unique_ptr<Asset::ShaderData> GraphicsManager::CreateShaderDataImpl(
        const Bytes& bytes, const Name& name)
    {
        const vk::ShaderModuleCreateInfo createInfo(
            {},
            bytes.size(),
            reinterpret_cast<const uint32_t*>(bytes.data()));
        auto created = device->createShaderModule(createInfo);
        return std::make_unique<ShaderAssetDataImplementation>(created, device);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateMainSurfaceDataImpl(void* window)
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

        return CreateSurfaceDataCommon(std::move(surface), *queueIndices, graphicsQueue, presentQueue);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateSurfaceDataImpl(void* window)
    {
        auto surface = CreateSurface(window, instance);
        const auto queueIndices = SuitableQueueFamilies(physicalDevice, surface.get());
        if (!queueIndices)
            throw GraphicsError("Could not create surface.");
        const auto presentQueue = device->getQueue(queueIndices->presentFamily, 0);
        return CreateSurfaceDataCommon(std::move(surface), *queueIndices, graphicsQueue, presentQueue);
    }
    bool GraphicsManager::ShouldReconstructInternals() const
    {
        return false;
    }

    void GraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
    {
        const auto handleSurface = [&](SurfaceDataImplementation& data)
        {
            const auto queueIndices = SuitableQueueFamilies(physicalDevice, data.Underlying());
            data.Reinitialize(*queueIndices);
        };

        handleSurface(*objects.mainSurface->Data<SurfaceDataImplementation>());

        for (auto& surface : objects.ancillarySurfaces)
            handleSurface(*surface->Data<SurfaceDataImplementation>());
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
        auto debugCreateInfo = debug.CreateInfo();
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
        std::optional<std::uint32_t> graphicsFamily;
        std::optional<std::uint32_t> presentFamily;

        const auto availableQueueFamilyProperties = physicalDevice.getQueueFamilyProperties();
        std::uint32_t i = 0;
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

    std::unique_ptr<SurfaceData> GraphicsManager::CreateSurfaceDataCommon(
        vk::UniqueSurfaceKHR&& underlying,
        QueueFamilyIndices queueIndices,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue)
    {
        return std::make_unique<SurfaceDataImplementation>(
            device,
            physicalDevice,
            std::move(underlying),
            sampler.get(),
            graphicsQueue,
            presentQueue,
            queueIndices,
            memoryProperties);
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
