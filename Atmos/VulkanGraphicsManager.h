#pragma once

#include "GraphicsManager.h"
#include "VulkanIncludes.h"
#include "VulkanQueueFamilyIndices.h"
#include "VulkanDebug.h"
#include "VulkanShaderCompiler.h"
#include "VulkanImage.h"
#include "VulkanStoredResource.h"
#include "VulkanSurfaceResource.h"
#include "VulkanMemoryPool.h"
#include "GraphicsError.h"

#define VK_KHR_WIN32_EXTENSION_NAME "VK_KHR_win32_surface"
#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"
#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

namespace Atmos::Render::Resource::Vulkan
{
    class BackingSurface;
}

namespace Atmos::Render::Vulkan
{
    class GraphicsManager final : public Render::GraphicsManager
    {
    public:
        GraphicsManager(Logging::Logger& logger);
        ~GraphicsManager();

        void SetFullscreen(bool set) override;
        void ChangeVerticalSync(bool set) override;
    protected:
        [[nodiscard]] std::unique_ptr<Asset::Resource::Image> CreateImageResourceImpl(
            const Bytes& bytes,
            const Name& name,
            const Spatial::Size2D& size) override;
        [[nodiscard]] std::unique_ptr<Asset::Resource::Shader> CreateShaderResourceImpl(
            const Bytes& bytes,
            const Name& name) override;
        [[nodiscard]] std::unique_ptr<Resource::Surface> CreateMainSurfaceResourceImpl(
            void* window) override;
        [[nodiscard]] std::unique_ptr<Resource::Surface> CreateSurfaceResourceImpl(
            void* window) override;
        [[nodiscard]] std::unique_ptr<Resource::Text> CreateTextResourceImpl(
            const Atmos::Buffer& buffer, const Spatial::Size2D& size) override;

        void StageImpl(const RenderImage& render) override;
        void StageImpl(const RenderLine& render) override;
        void StageImpl(const RenderRegion& render) override;
        void StageImpl(const RenderText& render) override;

        void StageImpl(const UpdateText& update) override;

        void DrawFrameImpl(
            Resource::Surface& surface,
            const Spatial::Point2D& mapPosition,
            const Color& backgroundColor,
            Diagnostics::Statistics::Profile& profile) override;

        void ResourceDestroyingImpl(Asset::Resource::Image& resource) override;
        void ResourceDestroyingImpl(Asset::Resource::Shader& resource) override;
        void ResourceDestroyingImpl(Resource::Surface& resource) override;
        void ResourceDestroyingImpl(Resource::Text& resource) override;

        Atmos::Buffer CompileShaderImpl(const File::Path& filePath) override;

        [[nodiscard]] bool ShouldReconstructInternals() const override;
        void ReconstructInternals(GraphicsReconstructionObjects objects) override;
    private:
        std::optional<MemoryPool> memoryPool;
    private:
        using StoredResourcePtr = std::unique_ptr<StoredResource>;
        std::list<StoredResourcePtr> storedResources;
        std::list<StoredResourcePtr> destroyedResources;

        void MoveToDestroyedResource(StoredResource& resource);
        void PruneResourcesImpl();
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
        vk::Device device;

        [[nodiscard]] vk::Device CreateDevice(
            vk::PhysicalDevice physicalDevice,
            QueueFamilyIndices queueFamilyIndices,
            const std::vector<const char*>& deviceExtensions,
            const std::vector<const char*>& instanceLayers);
    private:
        vk::UniqueCommandPool commandPool;
        vk::UniqueCommandBuffer imageCommandBuffer;
        vk::UniqueFence imageFence;

        [[nodiscard]] static vk::UniqueCommandPool CreateCommandPool(
            vk::Device device, QueueFamilyIndices queueFamilyIndices);
    private:
        vk::Queue graphicsQueue;

        [[nodiscard]] static std::optional<QueueFamilyIndices> SuitableQueueFamilies(
            vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    private:
        std::list<std::unique_ptr<Resource::Vulkan::BackingSurface>> surfaces;

        [[nodiscard]] std::unique_ptr<Resource::Surface> CreateSurfaceResourceCommon(
            vk::UniqueSurfaceKHR&& underlying,
            QueueFamilyIndices queueIndices,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue);

        [[nodiscard]] static vk::UniqueSurfaceKHR CreateSurface(void* window, vk::Instance instance);

        void WaitForSurfacesIdle();
    private:
        vk::UniqueSampler sampler;

        [[nodiscard]] static vk::UniqueSampler CreateSampler(vk::Device device);
    private:
        struct ImageDataPrototype
        {
            Atmos::Buffer buffer;
            Spatial::Size2D size;
        };

        std::unordered_map<Resource::Text*, UpdateText> textUpdates;

        template<class T>
        void StageRender(const T& render);

        void UpdateTexts();

        [[nodiscard]] std::vector<ImageData> CreateImageData(const std::vector<ImageDataPrototype>& prototypes);
    private:
        std::unique_ptr<Debug> debug;
    private:
        ShaderCompiler shaderCompiler;
    private:
        template<class To, class From>
        To& RequiredResource(From& from);
        template<class To, class From>
        const To& RequiredResource(const From& from);

        template<class Expected, class Available>
        static std::set<String> Unavailable(Expected expected, Available available);
    };

    template<class T>
    void GraphicsManager::StageRender(const T& render)
    {
        const auto& surface = RequiredResource<Resource::Vulkan::Surface>(*render.surface);
        surface.backing->Stage(render);
    }

    template<class To, class From>
    To& GraphicsManager::RequiredResource(From& from)
    {
        const auto casted = dynamic_cast<To*>(&from);
        if (!casted)
            throw GraphicsError("Unknown resource type.");
        return *casted;
    }

    template<class To, class From>
    const To& GraphicsManager::RequiredResource(const From& from)
    {
        const auto casted = dynamic_cast<const To*>(&from);
        if (!casted)
            throw GraphicsError("Unknown resource type.");
        return *casted;
    }
    
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