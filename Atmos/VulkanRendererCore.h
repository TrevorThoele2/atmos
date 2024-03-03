#pragma once

#include <optional>
#include <map>
#include "VulkanIncludes.h"

#include "VulkanDescriptorSetGroup.h"
#include "VulkanPipeline.h"

#include "Position3D.h"

namespace Atmos::Render::Vulkan
{
    template<class Discriminator, class Context>
    class RendererCore
    {
    public:
        struct DrawContext
        {
            std::vector<const Asset::Material*> materials;
            uint32_t count = 0;
            vk::CommandBuffer commandBuffer;
            uint32_t layerCount = 0;
            typename std::map<Position3D::Value, Context>::iterator currentLayer;
        };

        struct DiscriminatedDescriptorSet
        {
            Discriminator discriminator;
            std::uint32_t imageIndex;
            vk::DescriptorSet descriptorSet;
            DiscriminatedDescriptorSet(
                Discriminator discriminator, std::uint32_t imageIndex, vk::DescriptorSet descriptorSet);
        };
    public:
        RendererCore(
            std::shared_ptr<vk::Device> device,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            VertexInput vertexInput,
            const std::vector<DescriptorSetGroup::Definition>& descriptorSetGroupDefinitions,
            vk::PrimitiveTopology primitiveTopology,
            Asset::MaterialType materialAssetType,
            std::optional<Discriminator> onlyDiscrimination = {});

        void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent);
    public:
        template<class SetupDiscrimination>
        void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer, SetupDiscrimination setupDiscrimination);
        void End();
        [[nodiscard]] bool IsDone() const;
        [[nodiscard]] DrawContext* CurrentDrawContext();
    public:
        void AddDiscriminator(Discriminator discriminator);

        template<class Predicate>
        DiscriminatedDescriptorSet* DiscriminatedDescriptorSetFor(Predicate predicate);
    public:
        Context& AddContext(Position3D::Value key, const Context& value);
        Context* ContextFor(Position3D::Value value);
    public:
        Pipeline* PipelineFor(const Asset::Material& material);
    public:
        [[nodiscard]] Position3D::Value NextLayer() const;
        [[nodiscard]] size_t LayerCount() const;
    private:
        using LayeredContexts = std::map<Position3D::Value, Context>;
        LayeredContexts layeredContexts;
    private:
        std::optional<DrawContext> drawContext;
    private:
        struct DiscriminatorSource
        {
            Discriminator discriminator;
            std::uint32_t swapchainImageIndex;
            DiscriminatorSource(Discriminator discriminator, std::uint32_t swapchainImageIndex);
        };

        std::vector<DiscriminatorSource> discriminatorSources;
        std::vector<DescriptorSetGroup::Definition> descriptorSetGroupDefinitions;
        std::optional<DescriptorSetGroup> descriptorSets;
        std::vector<DiscriminatedDescriptorSet> discriminatedDescriptorSets;

        std::optional<Discriminator> onlyDiscrimination;

        template<class SetupDiscrimination>
        void SetupDiscriminatedDescriptorSet(SetupDiscrimination setupDiscrimination);
    private:
        VertexInput vertexInput;
        std::vector<Pipeline> pipelines;
        vk::PrimitiveTopology primitiveTopology;
        Pipeline CreatePipeline(
            const Asset::Material* material,
            vk::DescriptorSetLayout descriptorSetLayout,
            vk::Extent2D extent);
    private:
        std::shared_ptr<vk::Device> device;
        uint32_t swapchainImageCount = 0;
        vk::Extent2D extent;
        vk::RenderPass renderPass;
        vk::PhysicalDeviceMemoryProperties memoryProperties;
    private:
        Asset::MaterialType materialAssetType;
    };

    template<class Discriminator, class Context>
    RendererCore<Discriminator, Context>::DiscriminatedDescriptorSet::DiscriminatedDescriptorSet(
        Discriminator discriminator,
        std::uint32_t imageIndex,
        vk::DescriptorSet descriptorSet)
        :
        discriminator(discriminator),
        imageIndex(imageIndex),
        descriptorSet(descriptorSet)
    {}

    template<class Discriminator, class Context>
    RendererCore<Discriminator, Context>::RendererCore(
        std::shared_ptr<vk::Device> device,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        VertexInput vertexInput,
        const std::vector<DescriptorSetGroup::Definition>& descriptorSetGroupDefinitions,
        vk::PrimitiveTopology primitiveTopology,
        Asset::MaterialType materialAssetType,
        std::optional<Discriminator> onlyDiscrimination)
        :
        descriptorSetGroupDefinitions(descriptorSetGroupDefinitions),
        onlyDiscrimination(onlyDiscrimination),
        vertexInput(vertexInput),
        primitiveTopology(primitiveTopology),
        device(device),
        memoryProperties(memoryProperties),
        materialAssetType(materialAssetType)
    {}

    template<class Discriminator, class Context>
    void RendererCore<Discriminator, Context>::Initialize(
        uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        this->swapchainImageCount = swapchainImageCount;
        this->renderPass = renderPass;
        this->extent = extent;

        descriptorSets = DescriptorSetGroup(descriptorSetGroupDefinitions, *device);

        if (onlyDiscrimination)
        {
            discriminatorSources.clear();
            for (uint32_t swapchainImageIndex = 0; swapchainImageIndex < swapchainImageCount; ++swapchainImageIndex)
                discriminatorSources.emplace_back(*onlyDiscrimination, swapchainImageIndex);
        }
    }

    template<class Discriminator, class Context>
    template<class SetupDiscrimination>
    void RendererCore<Discriminator, Context>::Start(
        const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer, SetupDiscrimination setupDiscrimination)
    {
        if (layeredContexts.empty())
            return;

        drawContext = DrawContext();
        drawContext->materials = materials;
        drawContext->commandBuffer = commandBuffer;
        drawContext->currentLayer = layeredContexts.begin();

        SetupDiscriminatedDescriptorSet(setupDiscrimination);
    }

    template<class Discriminator, class Context>
    void RendererCore<Discriminator, Context>::End()
    {
        layeredContexts.clear();
        drawContext = {};

        descriptorSets->Reset();
        if (!onlyDiscrimination)
            discriminatorSources.clear();
    }

    template<class Discriminator, class Context>
    bool RendererCore<Discriminator, Context>::IsDone() const
    {
        return !drawContext || drawContext->currentLayer == layeredContexts.end();
    }

    template<class Discriminator, class Context>
    auto RendererCore<Discriminator, Context>::CurrentDrawContext() -> DrawContext*
    {
        return drawContext
            ? &*drawContext
            : nullptr;
    }

    template<class Discriminator, class Context>
    void RendererCore<Discriminator, Context>::AddDiscriminator(Discriminator discriminator)
    {
        for (auto& source : discriminatorSources)
            if (source.discriminator == discriminator)
                return;

        for (uint32_t swapchainImageIndex = 0; swapchainImageIndex < swapchainImageCount; ++swapchainImageIndex)
            discriminatorSources.emplace_back(discriminator, swapchainImageIndex);
    }

    template<class Discriminator, class Context>
    template<class Predicate>
    auto RendererCore<Discriminator, Context>::DiscriminatedDescriptorSetFor(Predicate predicate)
        -> DiscriminatedDescriptorSet*
    {
        auto found = std::find_if(
            discriminatedDescriptorSets.begin(),
            discriminatedDescriptorSets.end(),
            predicate);
        return (found == discriminatedDescriptorSets.end())
            ? nullptr
            : &*found;
    }

    template<class Discriminator, class Context>
    Context& RendererCore<Discriminator, Context>::AddContext(Position3D::Value key, const Context& value)
    {
        return layeredContexts.emplace(key, value).first->second;
    }

    template<class Discriminator, class Context>
    auto RendererCore<Discriminator, Context>::ContextFor(Position3D::Value value) -> Context*
    {
        auto found = layeredContexts.find(value);
        if (found == layeredContexts.end())
            return nullptr;

        return &found->second;
    }

    template<class Discriminator, class Context>
    auto RendererCore<Discriminator, Context>::PipelineFor(const Asset::Material& material) -> Pipeline*
    {
        for (auto& pipeline : pipelines)
            if (pipeline.material == &material)
                return &pipeline;

        return nullptr;
    }

    template<class Discriminator, class Context>
    Position3D::Value RendererCore<Discriminator, Context>::NextLayer() const
    {
        return drawContext->currentLayer->first;
    }

    template<class Discriminator, class Context>
    size_t RendererCore<Discriminator, Context>::LayerCount() const
    {
        return layeredContexts.size();
    }

    template<class Discriminator, class Context>
    RendererCore<Discriminator, Context>::DiscriminatorSource::DiscriminatorSource(
        Discriminator discriminator, std::uint32_t swapchainImageIndex)
        :
        discriminator(discriminator), swapchainImageIndex(swapchainImageIndex)
    {}

    template <class Discriminator, class Context>
    template<class SetupDiscrimination>
    void RendererCore<Discriminator, Context>::SetupDiscriminatedDescriptorSet(
        SetupDiscrimination setupDiscrimination)
    {
        const auto iterateDiscriminatorSources = [this](auto onEach)
        {
            for (std::uint32_t i = 0; i < discriminatorSources.size(); ++i)
            {
                const auto& source = discriminatorSources[i];
                const auto descriptorSet = descriptorSets->At(i);
                onEach(source, descriptorSet, i);
            }
        };

        const auto updatePipeline = [this, &iterateDiscriminatorSources](Pipeline& pipeline)
        {
            iterateDiscriminatorSources(
                [this, &pipeline](
                    const DiscriminatorSource& source, vk::DescriptorSet descriptorSet, std::uint32_t)
                {
                    pipeline.uniformBufferDescriptors[source.swapchainImageIndex].Update(descriptorSet, *device);
                });
        };

        const auto requiredDescriptorSetSize = discriminatorSources.size();
        if (descriptorSets->Size() < requiredDescriptorSetSize)
        {
            pipelines.clear();
            pipelines.reserve(drawContext->materials.size());
            discriminatedDescriptorSets.clear();
            discriminatedDescriptorSets.reserve(requiredDescriptorSetSize);
            descriptorSets->Reserve(requiredDescriptorSetSize);

            iterateDiscriminatorSources(
                [this, setupDiscrimination](
                    const DiscriminatorSource& source, vk::DescriptorSet descriptorSet, std::uint32_t)
                {
                    setupDiscrimination(source.discriminator, descriptorSet);
                    discriminatedDescriptorSets.emplace_back(
                        source.discriminator, source.swapchainImageIndex, descriptorSet);
                });

            for (auto& material : drawContext->materials)
            {
                if (material->Type() != materialAssetType)
                    continue;

                pipelines.push_back(CreatePipeline(material, descriptorSets->DescriptorSetLayout(), extent));
                auto& pipeline = pipelines.back();

                updatePipeline(pipeline);
            }
        }
        else
        {
            iterateDiscriminatorSources(
                [this, setupDiscrimination](
                    const DiscriminatorSource& source, vk::DescriptorSet descriptorSet, std::uint32_t index)
                {
                    auto& discriminatedDescriptorSet = discriminatedDescriptorSets[index];
                    discriminatedDescriptorSet.discriminator = source.discriminator;
                    setupDiscrimination(discriminatedDescriptorSet.discriminator, descriptorSet);
                });

            for (auto& pipeline : pipelines)
                updatePipeline(pipeline);
        }
    }

    template<class Discriminator, class Context>
    auto RendererCore<Discriminator, Context>::CreatePipeline(
        const Asset::Material* material,
        vk::DescriptorSetLayout descriptorSetLayout,
        vk::Extent2D extent)

        -> Pipeline
    {
        return Pipeline
        {
            *material,
            *device,
            swapchainImageCount,
            memoryProperties,
            { descriptorSetLayout },
            renderPass,
            vertexInput,
            extent,
            primitiveTopology,
            {}
        };
    }
}