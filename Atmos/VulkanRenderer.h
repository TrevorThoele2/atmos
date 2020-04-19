#pragma once

#include "VulkanIncludes.h"
#include "VulkanStagedBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanLineShaders.h"
#include "VulkanVertexInput.h"
#include "VulkanPipelineAssembly.h"
#include "VulkanUniformBufferDescriptor.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include <map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ImageRender.h"
#include "LineRender.h"
#include "ScreenSize.h"

#include "Event.h"

#undef CreateSemaphore

namespace Atmos::Render::Vulkan
{
    class Renderer final
    {
    public:
        Event<> onOutOfDate;
    public:
        Renderer(
            std::shared_ptr<vk::Device> device,
            vk::Sampler sampler,
            LineShaders lineShaders,
            vk::CommandPool commandPool,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            Arca::Reliquary& reliquary);
        ~Renderer();

        void Initialize(
            vk::SwapchainKHR swapchain,
            std::vector<vk::Image> images,
            std::vector<vk::ImageView> imageViews,
            vk::Format imageFormat,
            vk::Extent2D extent);
    public:
        void StageRender(const ImageRender& imageRender);
        void StageRender(const LineRender& lineRender);

        void DrawFrame(const ScreenSize& cameraSize);
    private:
        std::shared_ptr<vk::Device> device;
    private:
        static const int maxFramesInFlight = 2;
        size_t currentFrame = 0;
        size_t previousFrame = 1;
    private:
        struct TexturedVertex
        {
            glm::vec3 position;
            alignas(16) glm::vec2 texture;
            glm::vec4 color;
        };

        struct Quad
        {
            std::array<TexturedVertex, 4> vertices;
            Quad(const std::array<TexturedVertex, 4>& vertices);
        };

        static const int quadStride = 1000;
        static inline const std::array<uint16_t, 6> quadIndices = { 0, 2, 1, 2, 3, 1 };
    private:
        struct LineVertex
        {
            glm::vec3 position;
            alignas(16) glm::vec4 color;
        };

        struct Line
        {
            std::vector<LineVertex> vertices;
            Color color;
            float z;
            LineWidth width;
            Line(const std::vector<LineVertex>& points, const Color& color, float z, LineWidth width);
        };

        static const int lineStride = 5000;
    private:
        struct Layer
        {
            Position3D::Value z;

            struct QuadContext
            {
                struct QuadGroup
                {
                    std::vector<Quad> quads;
                    CombinedImageSamplerDescriptor descriptor;
                    explicit QuadGroup(CombinedImageSamplerDescriptor descriptor) : descriptor(descriptor)
                    {}
                };

                std::map<const Asset::ImageAsset*, QuadGroup> quadGroups;
                Pipeline* pipeline;
                explicit QuadContext(Pipeline& pipeline) : pipeline(&pipeline)
                {}

                QuadGroup* QuadGroupFor(const Asset::ImageAsset* imageAsset);
            };
            std::vector<QuadContext> quadContexts;

            struct LineContext
            {
                std::vector<Line> lines;
            };
            std::map<LineWidth, LineContext> lineContexts;

            explicit Layer(Position3D::Value z);

            QuadContext& ContextFor(Pipeline& pipeline);
            LineContext& ContextFor(LineWidth width);
        };

        std::vector<Layer> layers;
        Layer& LayerFor(Position3D::Value z);

        static Position3D::Value AtmosToVulkanZ(Position3D::Value input);
    private:
        void DrawAll(
            vk::CommandBuffer commandBuffer,
            std::uint32_t currentImage,
            glm::vec2 cameraSize);
        void DrawAllQuads(
            const std::vector<Quad>& quads,
            Pipeline& pipeline,
            CombinedImageSamplerDescriptor descriptor,
            vk::CommandBuffer commandBuffer,
            std::uint32_t currentImage,
            glm::vec2 cameraSize);
        void DrawAllLines(
            const std::vector<Line>& lines,
            LineWidth width,
            vk::CommandBuffer commandBuffer,
            std::uint32_t currentImage,
            glm::vec2 cameraSize);
        static void ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer);
    private:
        vk::SwapchainKHR swapchain;

        std::vector<vk::Image> images;
        std::vector<vk::ImageView> imageViews;

        vk::UniqueRenderPass renderPass;
        std::vector<vk::UniqueFramebuffer> framebuffers;
        vk::Extent2D extent;

        std::optional<PipelineAssembly> materialAssembly;
        std::optional<PipelineAssembly> lineAssembly;

        std::optional<UniformBufferDescriptor> materialUniformBufferDescriptor;
        std::optional<UniformBufferDescriptor> lineUniformBufferDescriptor;
    private:
        std::vector<Pipeline> materialPipelines;

        LineShaders lineShaders;
        Pipeline linePipeline;

        std::vector<Pipeline> CreateAllMaterialPipelines(
            const std::vector<const Asset::MaterialAsset*>& materials,
            vk::Extent2D extent);

        Pipeline CreateLinePipeline(
            vk::Extent2D extent);

        Pipeline* PipelineFor(const Asset::MaterialAsset& material);

        std::vector<Pipeline*> AllPipelines();

        static VertexInput TexturedVertexInput();
        static VertexInput LineVertexInput();
    private:
        [[nodiscard]] static vk::UniqueRenderPass CreateRenderPass(
            vk::Device device, vk::Format swapchainImageFormat);

        [[nodiscard]] static std::vector<vk::UniqueFramebuffer> CreateFramebuffers(
            vk::Device device, const std::vector<vk::ImageView>& imageViews, vk::RenderPass renderPass, vk::Extent2D extent);

        [[nodiscard]] static vk::UniquePipelineLayout CreatePipelineLayout(
            vk::Device device, vk::DescriptorSetLayout descriptorSetLayout);

        [[nodiscard]] static vk::UniquePipeline CreatePipeline(
            vk::Device device,
            const std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages,
            vk::PipelineLayout layout,
            vk::RenderPass renderPass,
            VertexInput vertexInput,
            vk::Extent2D swapchainExtent,
            vk::PrimitiveTopology primitiveTopology,
            std::vector<vk::DynamicState> dynamicStates = {});

        [[nodiscard]] static vk::PipelineShaderStageCreateInfo ShaderStageCreateInfo(
            const Asset::ShaderAsset& shaderAsset, vk::ShaderStageFlagBits shaderType);
    private:
        vk::Sampler sampler;

        vk::PhysicalDeviceMemoryProperties memoryProperties;

        static glm::vec4 AtmosToVulkanColor(const Color& color);
    private:
        StagedBuffer texturedVertexBuffer;
        static const int texturedVertexStride = quadStride * 4;

        StagedBuffer texturedIndexBuffer;
        static const int texturedIndexStride = quadStride * 6;

        StagedBuffer lineVertexBuffer;
        static const int lineVertexStride = lineStride;

        [[nodiscard]] static std::vector<Buffer> CreateUniformBuffers(
            vk::Device device, vk::PhysicalDeviceMemoryProperties memoryProperties, size_t size);
    private:
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        vk::CommandPool commandPool;
        std::vector<vk::UniqueCommandBuffer> commandBuffers;

        [[nodiscard]] static std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(
            vk::Device device, vk::CommandPool commandPool, size_t size);
    private:
        std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
        std::vector<vk::UniqueFence> inFlightFences;
        std::vector<vk::Fence> imagesInFlight;

        [[nodiscard]] static std::vector<vk::UniqueSemaphore> CreateSemaphores(vk::Device device, size_t count);
        [[nodiscard]] static std::vector<vk::UniqueFence> CreateFences(vk::Device device, size_t count);
    private:
        Arca::Reliquary* reliquary;
    };
}