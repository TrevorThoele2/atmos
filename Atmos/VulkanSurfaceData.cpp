#include "VulkanSurfaceData.h"

#include "VulkanCreateImageView.h"

namespace Atmos::Render::Vulkan
{
    SurfaceDataImplementation::SurfaceDataImplementation(
        std::shared_ptr<vk::Device> device,
        vk::PhysicalDevice physicalDevice,
        vk::UniqueSurfaceKHR&& underlying,
        vk::Sampler sampler,
        LineShaders lineShaders,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue,
        vk::CommandPool commandPool,
        QueueFamilyIndices queueFamilyIndices,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        Arca::Reliquary& reliquary)
        :
        device(device),
        physicalDevice(physicalDevice),
        sampler(sampler),
        underlying(std::move(underlying)),
        commandPool(commandPool)
    {
        renderer = std::make_unique<Renderer>(
            device,
            sampler,
            lineShaders,
            commandPool,
            graphicsQueue,
            presentQueue,
            memoryProperties,
            reliquary);

        Initialize(queueFamilyIndices);

        renderer->onOutOfDate.Subscribe([this, queueFamilyIndices]()
            {
                Reinitialize(queueFamilyIndices);
            });
    }

    SurfaceDataImplementation::~SurfaceDataImplementation()
    {
        renderer.reset();
        imageViews.clear();
        swapchain.reset();
        underlying.reset();
    }

    void SurfaceDataImplementation::Reinitialize(QueueFamilyIndices queueFamilyIndices)
    {
        Initialize(queueFamilyIndices);
    }

    void SurfaceDataImplementation::Initialize(QueueFamilyIndices queueFamilyIndices)
    {
        device->waitIdle();

        const auto swapchainSupportDetails = SwapchainSupport(physicalDevice, underlying.get());

        auto swapchainDetails = CreateSwapchain(
            queueFamilyIndices, swapchainSupportDetails, *device, underlying.get(), swapchain);

        const auto imageFormat = swapchainDetails.imageFormat;
        const auto extent = swapchainDetails.extent;

        swapchain = std::move(swapchainDetails.swapchain);
        swapchainExtent = swapchainDetails.extent;

        auto images = device->getSwapchainImagesKHR(swapchain.get());

        imageViews.clear();
        imageViews.reserve(images.size());
        for (auto& image : images)
            imageViews.push_back(CreateImageView(image, *device, swapchainDetails.imageFormat));
        nonUniqueImageViews.clear();
        nonUniqueImageViews.reserve(imageViews.size());
        for (auto& imageView : imageViews)
            nonUniqueImageViews.push_back(imageView.get());

        renderer->Initialize(
            swapchain.get(),
            images,
            nonUniqueImageViews,
            imageFormat,
            extent);
    }

    void SurfaceDataImplementation::StageRender(const ImageRender& imageRender)
    {
        renderer->StageRender(imageRender);
    }

    void SurfaceDataImplementation::StageRender(const LineRender& lineRender)
    {
        renderer->StageRender(lineRender);
    }

    void SurfaceDataImplementation::DrawFrame(const Color& backgroundColor)
    {
        renderer->DrawFrame(Size());
    }

    ScreenSize SurfaceDataImplementation::Size() const
    {
        return
        {
            static_cast<ScreenSize::Dimension>(swapchainExtent.width),
            static_cast<ScreenSize::Dimension>(swapchainExtent.height)
        };
    }

    vk::SurfaceKHR SurfaceDataImplementation::Underlying() const
    {
        return underlying.get();
    }

    auto SurfaceDataImplementation::CreateSwapchain(
        QueueFamilyIndices queueFamilyIndices,
        SwapchainSupportDetails support,
        vk::Device device,
        vk::SurfaceKHR surface,
        const vk::UniqueSwapchainKHR& swapchain)
        -> SwapchainDetails
    {
        const auto surfaceFormat = ChooseSwapChainFormat(support.formats);
        const auto presentMode = ChooseSwapChainPresentMode(support.presentModes);
        const auto extent = support.capabilities.currentExtent;
        const auto imageCount = support.capabilities.maxImageCount > 0 && support.capabilities.minImageCount + 1 > support.capabilities.maxImageCount
            ? support.capabilities.maxImageCount
            : support.capabilities.minImageCount;

        const auto previousSwapchain = swapchain
            ? swapchain.get()
            : std::optional<vk::SwapchainKHR>{};

        vk::SwapchainCreateInfoKHR createInfo(
            {},
            surface,
            imageCount,
            surfaceFormat.format,
            surfaceFormat.colorSpace,
            extent,
            1,
            vk::ImageUsageFlagBits::eColorAttachment,
            {},
            {},
            {},
            support.capabilities.currentTransform,
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            presentMode,
            VK_TRUE,
            previousSwapchain ? *previousSwapchain : nullptr);
        if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily)
        {
            uint32_t passQueueFamilyIndices[] = { queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentFamily };

            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = passQueueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        return { device.createSwapchainKHRUnique(createInfo), surfaceFormat.format, extent };
    }

    vk::SurfaceFormatKHR SurfaceDataImplementation::ChooseSwapChainFormat(
        const std::vector<vk::SurfaceFormatKHR>& surfaceFormats)
    {
        for (const auto& surfaceFormat : surfaceFormats)
            if (surfaceFormat.format == vk::Format::eB8G8R8A8Srgb
                && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                return surfaceFormat;
            }

        return surfaceFormats[0];
    }

    vk::PresentModeKHR SurfaceDataImplementation::ChooseSwapChainPresentMode(
        const std::vector<vk::PresentModeKHR>& surfacePresentModes)
    {
        for (const auto& surfacePresentMode : surfacePresentModes)
            if (surfacePresentMode == vk::PresentModeKHR::eMailbox)
                return surfacePresentMode;

        return vk::PresentModeKHR::eFifo;
    }
}