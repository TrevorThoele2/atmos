#pragma once

#include "VulkanKeyedDescriptorSetGroup.h"
#include "VulkanObjectLayering.h"
#include "VulkanPipelineGroup.h"
#include "VulkanDrawContext.h"
#include "VulkanUniversalData.h"

namespace Atmos::Render::Vulkan
{
    template<class Key, class Context, class DrawContextAddition>
    class RendererCoreBase
    {
    public:
        using DrawContext = DrawContext<Context, DrawContextAddition>;
        using Draw = std::function<void(Context&, DrawContext&, uint32_t, UniversalData)>;
    public:
        RendererCoreBase(
            std::shared_ptr<vk::Device> device,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            const std::vector<DescriptorSetGroup::Definition>& descriptorSetDefinitions,
            VertexInput vertexInput,
            vk::PrimitiveTopology primitiveTopology,
            Asset::MaterialType materialType,
            Draw&& draw);
        virtual ~RendererCoreBase() = 0;

        void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent);

        void DrawNextLayer(uint32_t currentImage, UniversalData universalData);
        void End();
        [[nodiscard]] bool IsDone() const;

        Context& AddContext(Spatial::Point3D::Value z, Context&& context);
        Context* ContextFor(Spatial::Point3D::Value z);

        [[nodiscard]] std::vector<Pipeline>* PipelinesFor(const Asset::Material& material);
        [[nodiscard]] vk::PipelineLayout PipelineLayout() const;

        [[nodiscard]] Spatial::Point3D::Value NextLayer() const;
        [[nodiscard]] size_t LayerCount() const;
    protected:
        using DescriptorSets = KeyedDescriptorSetGroup<Key>;
        DescriptorSets descriptorSets;

        using ObjectLayering = ObjectLayering<Context>;
        ObjectLayering layers;
        PipelineGroup pipelines;
        std::shared_ptr<vk::Device> device;

        void StartCommon(
            const std::vector<const Asset::Material*>& materials,
            vk::CommandBuffer commandBuffer,
            bool recreatePipelines);
    private:
        std::optional<DrawContext> drawContext;

        uint32_t swapchainImageCount = 0;
        vk::RenderPass renderPass;
        vk::Extent2D extent;

        Draw draw;
    };

    template<class Key, class Context, class DrawContextAddition>
    RendererCoreBase<Key, Context, DrawContextAddition>::RendererCoreBase(
        std::shared_ptr<vk::Device> device,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        const std::vector<DescriptorSetGroup::Definition>& descriptorSetDefinitions,
        VertexInput vertexInput,
        vk::PrimitiveTopology primitiveTopology,
        Asset::MaterialType materialType,
        Draw&& draw)
        :
        descriptorSets(descriptorSetDefinitions, *device),
        pipelines(device, memoryProperties, vertexInput, primitiveTopology, materialType),
        device(device),
        draw(std::move(draw))
    {}

    template <class Key, class Context, class DrawContextAddition>
    RendererCoreBase<Key, Context, DrawContextAddition>::~RendererCoreBase() = default;

    template<class Key, class Context, class DrawContextAddition>
    void RendererCoreBase<Key, Context, DrawContextAddition>::Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        this->swapchainImageCount = swapchainImageCount;
        this->renderPass = renderPass;
        this->extent = extent;

        descriptorSets.Clear();
        descriptorSets.Initialize(swapchainImageCount);
    }

    template<class Key, class Context, class DrawContextAddition>
    void RendererCoreBase<Key, Context, DrawContextAddition>::DrawNextLayer(
        uint32_t currentImage, UniversalData universalData)
    {
        auto& context = drawContext->currentLayer->second;

        draw(context, *drawContext, currentImage, universalData);

        ++drawContext->layerCount;
        ++drawContext->currentLayer;
    }

    template<class Key, class Context, class DrawContextAddition>
    void RendererCoreBase<Key, Context, DrawContextAddition>::End()
    {
        drawContext = {};

        layers.Clear();
        descriptorSets.Clear();
    }

    template<class Key, class Context, class DrawContextAddition>
    bool RendererCoreBase<Key, Context, DrawContextAddition>::IsDone() const
    {
        return !drawContext || drawContext->currentLayer == layers.end();
    }

    template<class Key, class Context, class DrawContextAddition>
    Context& RendererCoreBase<Key, Context, DrawContextAddition>::AddContext(
        Spatial::Point3D::Value z, Context&& context)
    {
        return layers.Add(z, std::move(context));
    }

    template<class Key, class Context, class DrawContextAddition>
    Context* RendererCoreBase<Key, Context, DrawContextAddition>::ContextFor(
        Spatial::Point3D::Value z)
    {
        return layers.Find(z);
    }

    template<class Key, class Context, class DrawContextAddition>
    std::vector<Pipeline>* RendererCoreBase<Key, Context, DrawContextAddition>::PipelinesFor(const Asset::Material& material)
    {
        return pipelines.Find(material);
    }

    template<class Key, class Context, class DrawContextAddition>
    vk::PipelineLayout RendererCoreBase<Key, Context, DrawContextAddition>::PipelineLayout() const
    {
        return pipelines.Layout();
    }

    template<class Key, class Context, class DrawContextAddition>
    Spatial::Point3D::Value RendererCoreBase<Key, Context, DrawContextAddition>::NextLayer() const
    {
        return drawContext->currentLayer->first;
    }

    template<class Key, class Context, class DrawContextAddition>
    size_t RendererCoreBase<Key, Context, DrawContextAddition>::LayerCount() const
    {
        return layers.Count();
    }

    template<class Key, class Context, class DrawContextAddition>
    void RendererCoreBase<Key, Context, DrawContextAddition>::StartCommon(
        const std::vector<const Asset::Material*>& materials,
        vk::CommandBuffer commandBuffer,
        bool recreatePipelines)
    {
        if (recreatePipelines)
            pipelines.Recreate(materials, { descriptorSets.Layout() }, swapchainImageCount, renderPass, extent);

        for (auto& pipelineGroupEntry : pipelines)
            for(auto& pipeline : pipelineGroupEntry.second)
                for (auto& descriptorSet : descriptorSets)
                    pipeline.uniformBufferDescriptors[descriptorSet.imageIndex].Update(
                        descriptorSet.descriptorSet, *device);

        drawContext = DrawContext();
        drawContext->materials = materials;
        drawContext->commandBuffer = commandBuffer;
        drawContext->currentLayer = layers.begin();
    }

    template<class Key, class Context, class DrawContextAddition>
    class RendererCore final : public RendererCoreBase<Key, Context, DrawContextAddition>
    {
    private:
        using BaseT = RendererCoreBase<Key, Context, DrawContextAddition>;

        using DescriptorSets = typename BaseT::DescriptorSets;
    public:
        using KeyedSet = typename DescriptorSets::KeyedSet;
    public:
        using BaseT::BaseT;

        template<class SetupKey>
        void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer, SetupKey setupKey);
        void AddKey(Key key);
        KeyedSet* KeyedSetFor(Key key, uint32_t imageIndex);
    protected:
        using BaseT::descriptorSets;
        using BaseT::layers;
        using BaseT::pipelines;
        using BaseT::device;
        using BaseT::StartCommon;
    };

    template<class Key, class Context, class DrawContextAddition>
    template<class SetupKey>
    void RendererCore<Key, Context, DrawContextAddition>::Start(
        const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer, SetupKey setupKey)
    {
        if (layers.Empty())
            return;

        const auto recreatedDescriptorSets = descriptorSets.UpdateSets(setupKey);
        StartCommon(materials, commandBuffer, recreatedDescriptorSets);
    }

    template<class Key, class Context, class DrawContextAddition>
    void RendererCore<Key, Context, DrawContextAddition>::AddKey(Key key)
    {
        descriptorSets.AddKey(key);
    }

    template<class Key, class Context, class DrawContextAddition>
    auto RendererCore<Key, Context, DrawContextAddition>::KeyedSetFor(Key key, uint32_t imageIndex) -> KeyedSet*
    {
        return descriptorSets.KeyedSetFor(key, imageIndex);
    }

    template<class Context, class DrawContextAddition>
    class RendererCore<void, Context, DrawContextAddition> final : public RendererCoreBase<void, Context, DrawContextAddition>
    {
    private:
        using BaseT = RendererCoreBase<void, Context, DrawContextAddition>;

        using DescriptorSets = typename BaseT::DescriptorSets;
    public:
        using KeyedSet = typename DescriptorSets::KeyedSet;
    public:
        using BaseT::BaseT;

        void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer);
        KeyedSet* KeyedSetFor(uint32_t imageIndex);
    protected:
        using BaseT::descriptorSets;
        using BaseT::layers;
        using BaseT::pipelines;
        using BaseT::device;
        using BaseT::StartCommon;
    };

    template<class Context, class DrawContextAddition>
    void RendererCore<void, Context, DrawContextAddition>::Start(
        const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer)
    {
        if (layers.Empty())
            return;

        const auto recreatedDescriptorSets = descriptorSets.UpdateSets();
        StartCommon(materials, commandBuffer, recreatedDescriptorSets);
    }

    template<class Context, class DrawContextAddition>
    auto RendererCore<void, Context, DrawContextAddition>::KeyedSetFor(uint32_t imageIndex) -> KeyedSet*
    {
        return descriptorSets.KeyedSetFor(imageIndex);
    }
}