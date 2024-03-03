#include "VulkanSurfaceResource.h"

#include "VulkanCreateImageView.h"

#include "Camera.h"

namespace Atmos::Render::Resource::Vulkan
{
    Surface::Surface(
        std::shared_ptr<vk::Device> device,
        vk::PhysicalDevice physicalDevice,
        vk::UniqueSurfaceKHR&& underlying,
        vk::Sampler sampler,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue,
        QueueFamilyIndices queueFamilyIndices,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        device(device),
        physicalDevice(physicalDevice),
        sampler(sampler),
        underlying(std::move(underlying))
    {
        renderer = std::make_unique<MasterRenderer>(
            device,
            sampler,
            graphicsQueue,
            presentQueue,
            queueFamilyIndices.graphicsFamily,
            memoryProperties);

        Initialize(queueFamilyIndices);

        renderer->onOutOfDate.Subscribe([this, queueFamilyIndices]()
            {
                Reinitialize(queueFamilyIndices);
            });
    }

    Surface::~Surface()
    {
        renderer.reset();
        imageViews.clear();
        swapchain.reset();
        underlying.reset();
    }

    void Surface::Reinitialize(QueueFamilyIndices queueFamilyIndices)
    {
        Initialize(queueFamilyIndices);
    }

    void Surface::Initialize(QueueFamilyIndices queueFamilyIndices)
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
            imageViews.push_back(CreateImageView(
                image,
                *device,
                swapchainDetails.imageFormat,
                0,
                1));
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

    void Surface::StageRender(const ImageRender& imageRender)
    {
        renderer->StageRender(imageRender);
    }

    void Surface::StageRender(const LineRender& lineRender)
    {
        renderer->StageRender(lineRender);
    }

    void Surface::StageRender(const RegionRender& regionRender)
    {
        renderer->StageRender(regionRender);
    }

    void Surface::DrawFrame(Arca::Reliquary& reliquary, const Color& backgroundColor)
    {
        const auto camera = Arca::Index<Camera>(reliquary);
        const auto mapPosition = ScreenPosition
        {
            ScreenPosition::Value(camera->center.x),
            ScreenPosition::Value(camera->center.y)
        };
        renderer->DrawFrame(reliquary, Size(), mapPosition);
    }

    void Surface::WaitForIdle() const
    {
        renderer->WaitForIdle();
    }

    ScreenSize Surface::Size() const
    {
        return
        {
            static_cast<ScreenSize::Dimension>(swapchainExtent.width),
            static_cast<ScreenSize::Dimension>(swapchainExtent.height)
        };
    }

    vk::SurfaceKHR Surface::Underlying() const
    {
        return underlying.get();
    }

    auto Surface::CreateSwapchain(
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

    vk::SurfaceFormatKHR Surface::ChooseSwapChainFormat(
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

    vk::PresentModeKHR Surface::ChooseSwapChainPresentMode(
        const std::vector<vk::PresentModeKHR>& surfacePresentModes)
    {
        for (const auto& surfacePresentMode : surfacePresentModes)
            if (surfacePresentMode == vk::PresentModeKHR::eMailbox)
                return surfacePresentMode;

        return vk::PresentModeKHR::eFifo;
    }
}