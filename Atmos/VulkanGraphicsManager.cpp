#include "VulkanGraphicsManager.h"

#include "VulkanSurfaceData.h"
#include "VulkanImageAssetData.h"
#include "VulkanShaderAssetData.h"

#include "VulkanMemory.h"
#include "VulkanSingleUseCommandBuffer.h"
#include "VulkanCreateImageView.h"
#include "VulkanSwapchainSupport.h"
#include "VulkanUtilities.h"

#include "AncillarySurface.h"
#include "MainSurface.h"
#include "GraphicsError.h"

#include "Log.h"

#include "WindowProvider.h"
#include "SimpleFile.h"

#include <Arca/Reliquary.h>

namespace Atmos::Render::Vulkan
{
    GraphicsManager::~GraphicsManager()
    {
        DestroyDebugMessenger();
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

    void GraphicsManager::InitializeImpl()
    {
        debugCallbackData.reliquary = &Reliquary();

#ifndef NDEBUG
        instanceLayers.push_back(VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME);
        instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

        ValidateRequiredInstanceExtensions();
        ValidateRequiredInstanceLayers();

        instance = CreateInstance();
        debugMessenger = CreateDebugMessenger(instance, debugCallbackData);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageDataImpl(
        const Bytes& bytes, const Name& name, const Size2D& size)
    {
        const auto width = static_cast<uint32_t>(size.width);
        const auto height = static_cast<uint32_t>(size.height);

        const auto format = vk::Format::eR8G8B8A8Srgb;

        const auto firstLayout = vk::ImageLayout::eUndefined;
        const auto secondLayout = vk::ImageLayout::eTransferDstOptimal;
        const auto thirdLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        const auto extent = vk::Extent3D(width, height, 1);
        const vk::ImageCreateInfo imageCreateInfo(
            {},
            vk::ImageType::e2D,
            format,
            extent,
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
            vk::SharingMode::eExclusive,
            {},
            {},
            firstLayout);
        auto image = device->createImageUnique(imageCreateInfo);

        const auto memoryRequirements = device->getImageMemoryRequirements(image.get());

        const vk::MemoryAllocateInfo memoryAllocationInfo(
            memoryRequirements.size,
            FindSuitableMemoryType(
                memoryRequirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                memoryProperties));

        auto imageMemory = device->allocateMemoryUnique(memoryAllocationInfo);

        device->bindImageMemory(image.get(), imageMemory.get(), 0);

        Buffer stagingBuffer(
            vk::DeviceSize(bytes.size()),
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            *device,
            memoryProperties);
        stagingBuffer.PushBytes(bytes, 0);

        TransitionImageLayout(
            image.get(),
            format,
            firstLayout,
            secondLayout);
        stagingBuffer.Copy(image.get(), width, height, *commandPool, graphicsQueue);
        TransitionImageLayout(
            image.get(),
            format,
            secondLayout,
            thirdLayout);

        auto imageView = CreateImageView(image.get(), *device, format);

        auto descriptor = CombinedImageSamplerDescriptor(
            imageView.get(), sampler.get(), vk::ImageLayout::eShaderReadOnlyOptimal, 1);

        return std::make_unique<ImageAssetDataImplementation>(
            std::move(image), std::move(imageMemory), std::move(imageView), descriptor, width, height);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderDataImpl(
        const Bytes& bytes, const Name& name, const String& entryPoint)
    {
        const vk::ShaderModuleCreateInfo createInfo(
            {},
            bytes.size(),
            reinterpret_cast<const uint32_t*>(bytes.data()));
        auto created = device->createShaderModule(createInfo);
        return std::make_unique<ShaderAssetDataImplementation>(created, entryPoint, device);
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

        defaultTexturedImageMaterial = CreateMaterial(
            { "TexturedImage.vert", "TexturedImageVertex.spv", "main" },
            { "TexturedImage.frag", "TexturedImageFragment.spv", "main" },
            "DefaultTexturedImage");

        lineShaders =
        {
            CreateShader({"Line.vert", "LineVertex.spv", "main"}),
            CreateShader({"Line.frag", "LineFragment.spv", "main"})
        };

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

    Arca::Index<Asset::ShaderAsset> GraphicsManager::CreateShader(ShaderCreateDescriptor descriptor)
    {
        const auto inputFilePath = std::filesystem::current_path() / "Shaders" / descriptor.inputName;
        const auto outputFilePath = std::filesystem::current_path() / "Shaders" / descriptor.outputName;

        shaderCompiler.Compile(inputFilePath, outputFilePath);

        SimpleInFile inFile(outputFilePath);
        const auto bytes = inFile.ReadBuffer();

        auto data = CreateShaderData(bytes, descriptor.inputName, descriptor.entryPoint);

        return Reliquary().Do<Arca::Create<Asset::ShaderAsset>>(
            descriptor.inputName,
            std::move(data));
    }

    Arca::Index<Asset::MaterialAsset> GraphicsManager::CreateMaterial(
        ShaderCreateDescriptor vertex,
        ShaderCreateDescriptor fragment,
        const String& materialName)
    {
        auto vertexShader = CreateShader(std::move(vertex));
        auto fragmentShader = CreateShader(std::move(fragment));

        return Reliquary().Do<Arca::Create<Asset::MaterialAsset>>(
            materialName,
            vertexShader,
            fragmentShader);
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
        auto debugMessengerCreateInfo = DebugMessengerCreateInfo(debugCallbackData);
        createInfo.pNext = &debugMessengerCreateInfo;

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

        if (!graphicsFamily || !presentFamily)
            return {};
        else
            return QueueFamilyIndices{ graphicsFamily.value(), presentFamily.value() };
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
            lineShaders,
            graphicsQueue,
            presentQueue,
            *commandPool,
            queueIndices,
            memoryProperties,
            Reliquary());
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
            vk::Filter::eLinear,
            vk::Filter::eLinear,
            vk::SamplerMipmapMode::eLinear,
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

    void GraphicsManager::TransitionImageLayout(
        vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
    {
        SingleUseCommandBuffer(*device, *commandPool, graphicsQueue,
            [image, oldLayout, newLayout](vk::CommandBuffer commandBuffer)
            {
                const vk::ImageSubresourceRange subresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
                vk::ImageMemoryBarrier barrier(
                    {},
                    {},
                    oldLayout,
                    newLayout,
                    VK_QUEUE_FAMILY_IGNORED,
                    VK_QUEUE_FAMILY_IGNORED,
                    image,
                    subresourceRange);

                vk::PipelineStageFlags sourceStage;
                vk::PipelineStageFlags destinationStage;

                if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
                {
                    barrier.srcAccessMask = {};
                    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

                    sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                    destinationStage = vk::PipelineStageFlagBits::eTransfer;
                }
                else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
                {
                    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

                    sourceStage = vk::PipelineStageFlagBits::eTransfer;
                    destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
                }
                else
                    throw GraphicsError("Unsupported layout transition.");

                commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr, barrier);
            });
    }

    VkDebugUtilsMessengerEXT GraphicsManager::CreateDebugMessenger(
        vk::Instance instance, DebugCallbackData& debugCallbackData)
    {
        const auto createInfo = DebugMessengerCreateInfo(debugCallbackData);
        const auto createDebugUtilsMessenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (!createDebugUtilsMessenger)
            throw GraphicsError("Could not retrieve debug messenger.");

        VkDebugUtilsMessengerEXT returnValue;
        if (IsError(createDebugUtilsMessenger(static_cast<VkInstance>(instance), &createInfo, nullptr, &returnValue)))
            throw GraphicsError("Could not create debug messenger.");
        return returnValue;
    }

    void GraphicsManager::DestroyDebugMessenger()
    {
        const auto destroyDebugUtilsMessenger = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (destroyDebugUtilsMessenger != nullptr) {
            destroyDebugUtilsMessenger(static_cast<VkInstance>(instance), debugMessenger, nullptr);
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT GraphicsManager::DebugMessengerCreateInfo(
        DebugCallbackData& debugCallbackData)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = &DebugCallback;
        createInfo.pUserData = &debugCallbackData;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        return createInfo;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsManager::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        Logging::Severity severity;
        if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            return VK_FALSE;

        switch(messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            severity = Logging::Severity::Verbose;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            severity = Logging::Severity::Information;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            severity = Logging::Severity::Warning;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            severity = Logging::Severity::Error;
            break;
        default:
            severity = Logging::Severity::Error;
            break;
        }

        const auto message = pCallbackData->pMessage ? pCallbackData->pMessage : String("No message.");

        auto data = static_cast<DebugCallbackData*>(pUserData);
        data->reliquary->Do(Logging::Log(
            String(message),
            severity));

        return VK_FALSE;
    }
}
