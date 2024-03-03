#pragma once

#include <optional>
#include <set>
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
            DiscriminatedDescriptorSet(Discriminator discriminator, std::uint32_t imageIndex, vk::DescriptorSet descriptorSet);
        };
    public:
        std::set<Discriminator> allDiscriminations;
    public:
        RendererCore(
            std::shared_ptr<vk::Device> device,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            VertexInput vertexInput,
            const std::vector<DescriptorSetGroup::Definition>& descriptorSetGroupDefinitions,
            vk::PrimitiveTopology primitiveTopology,
            Asset::MaterialType materialAssetType);

        void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent);

        template<class SetupDiscrimination>
        void AttemptReconstructDiscriminatedDescriptorSet(SetupDiscrimination setupDiscrimination);
    public:
        void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer);
        void End();
        [[nodiscard]] bool IsDone() const;
        [[nodiscard]] DrawContext* CurrentDrawContext();
    public:
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
        std::vector<const Asset::Material*> previousMaterialAssets;
    private:
        std::vector<DescriptorSetGroup::Definition> descriptorSetGroupDefinitions;
        std::optional<DescriptorSetGroup> descriptorSets;
        std::vector<DiscriminatedDescriptorSet> discriminatedDescriptorSets;
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
        Asset::MaterialType materialAssetType)
        :
        descriptorSetGroupDefinitions(descriptorSetGroupDefinitions),
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
    }

    template <class Discriminator, class Context>
    template<class SetupDiscrimination>
    void RendererCore<Discriminator, Context>::AttemptReconstructDiscriminatedDescriptorSet(
        SetupDiscrimination setupDiscrimination)
    {
        const auto size = allDiscriminations.size() * swapchainImageCount;
        if (descriptorSets->Size() < size || drawContext->materials != previousMaterialAssets)
        {
            pipelines.clear();
            discriminatedDescriptorSets.clear();
            descriptorSets->Reserve(size);

            auto descriptorSetIndex = 0;
            for (uint32_t swapchainImageIndex = 0; swapchainImageIndex < swapchainImageCount; ++swapchainImageIndex)
            {
                for (auto discrimination : allDiscriminations)
                {
                    auto descriptorSet = descriptorSets->At(descriptorSetIndex);

                    setupDiscrimination(discrimination, descriptorSet);

                    discriminatedDescriptorSets.emplace_back(discrimination, swapchainImageIndex, descriptorSet);
                    ++descriptorSetIndex;
                }
            }

            for (auto& material : drawContext->materials)
            {
                if (material->Type() != materialAssetType)
                    continue;

                pipelines.push_back(CreatePipeline(material, descriptorSets->DescriptorSetLayout(), extent));
                auto& pipeline = pipelines.back();

                descriptorSetIndex = 0;
                for (uint32_t swapchainImageIndex = 0; swapchainImageIndex < swapchainImageCount; ++swapchainImageIndex)
                {
                    for (auto discrimination : allDiscriminations)
                    {
                        pipeline.uniformBufferDescriptors[swapchainImageIndex].Update(
                            descriptorSets->At(descriptorSetIndex), *device);
                        ++descriptorSetIndex;
                    }
                }
            }
        }
    }

    template<class Discriminator, class Context>
    void RendererCore<Discriminator, Context>::Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer)
    {
        if (layeredContexts.empty())
            return;

        drawContext = DrawContext();
        drawContext->materials = materials;
        drawContext->commandBuffer = commandBuffer;
        drawContext->currentLayer = layeredContexts.begin();
    }

    template<class Discriminator, class Context>
    void RendererCore<Discriminator, Context>::End()
    {
        layeredContexts.clear();
        previousMaterialAssets = drawContext->materials;
        drawContext = {};

        descriptorSets->Reset();
        allDiscriminations.clear();
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