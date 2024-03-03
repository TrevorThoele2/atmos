#pragma once

#include "GraphicsManager.h"
#include "VulkanIncludes.h"
#include "VulkanShaderCompiler.h"
#include "VulkanQueueFamilyIndices.h"
#include "VulkanDebug.h"

#define VK_KHR_WIN32_EXTENSION_NAME "VK_KHR_win32_surface"
#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"
#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

namespace Atmos::Render::Vulkan
{
    class GraphicsManager final : public Render::GraphicsManager
    {
    public:
        ShaderCompiler shaderCompiler;
    public:
        GraphicsManager();

        [[nodiscard]] bool IsOk() const override;

        void SetFullscreen(bool set) override;
        void ChangeVerticalSync(bool set) override;
    protected:
        [[nodiscard]] std::unique_ptr<Asset::ImageData> CreateImageDataImpl(
            const Bytes& bytes,
            const Name& name,
            const Asset::ImageSize& size) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderData> CreateShaderDataImpl(
            const Bytes& bytes, const Name& name) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceDataImpl(
            void* window) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceDataImpl(
            void* window) override;

        [[nodiscard]] bool ShouldReconstructInternals() const override;
        void ReconstructInternals(GraphicsReconstructionObjects objects) override;
    private:
        vk::Instance instance;
        std::vector<const char*> instanceExtensions =
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };
        std::vector<const char*> instanceLayers;

        vk::Instance CreateInstance();
        void ValidateRequiredInstanceExtensions() const;
        void ValidateRequiredInstanceLayers() const;
    private:
        vk::PhysicalDevice physicalDevice = nullptr;
        vk::PhysicalDeviceMemoryProperties memoryProperties;
        std::vector<const char*> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        [[nodiscard]] static std::optional<vk::PhysicalDevice> PickPhysicalDevice(
            vk::SurfaceKHR mainSurface, vk::Instance instance, const std::vector<const char*>& deviceExtensions);

        [[nodiscard]] static bool HasRequiredDeviceExtensionProperties(
            vk::PhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions);
    private:
        std::shared_ptr<vk::Device> device;

        [[nodiscard]] static std::shared_ptr<vk::Device> CreateDevice(
            vk::Instance instance,
            vk::PhysicalDevice physicalDevice,
            QueueFamilyIndices queueFamilyIndices,
            const std::vector<const char*>& deviceExtensions,
            const std::vector<const char*>& instanceLayers);
    private:
        vk::UniqueCommandPool commandPool;

        [[nodiscard]] static vk::UniqueCommandPool CreateCommandPool(
            vk::Device device, QueueFamilyIndices queueFamilyIndices);
    private:
        vk::Queue graphicsQueue;

        [[nodiscard]] static std::optional<QueueFamilyIndices> SuitableQueueFamilies(
            vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    private:
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceDataCommon(
            vk::UniqueSurfaceKHR&& underlying,
            QueueFamilyIndices queueIndices,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue);

        [[nodiscard]] static vk::UniqueSurfaceKHR CreateSurface(void* window, vk::Instance instance);
    private:
        vk::UniqueSampler sampler;

        [[nodiscard]] static vk::UniqueSampler CreateSampler(vk::Device device);
    private:
        Debug debug;
    private:
        template<class Expected, class Available>
        static std::set<String> Unavailable(Expected expected, Available available);
    };

    template<class Expected, class Available>
    std::set<String> GraphicsManager::Unavailable(Expected expected, Available available)
    {
        std::set<String> expectedSet(expected.begin(), expected.end());
        std::set<String> availableSet(available.begin(), available.end());

        std::set<String> returnValue;
        std::set_difference(
            expectedSet.begin(), expectedSet.end(),
            availableSet.begin(), availableSet.end(),
            std::inserter(returnValue, returnValue.begin()));
        return returnValue;
    }
}