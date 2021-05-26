#pragma once

#include <unordered_map>
#include "VulkanConduit.h"
#include "MaterialAsset.h"

#include "VulkanInvalidConduit.h"

namespace Atmos::Render::Vulkan
{
    template<class MaterialT>
    class MappedConduits
    {
    public:
        using Group = std::vector<Conduit>;
    public:
        MappedConduits(
            std::shared_ptr<vk::Device> device,
            VertexInput vertexInput,
            vk::PrimitiveTopology primitiveTopology,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            std::vector<vk::DynamicState> dynamicStates,
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);

        void Add(Arca::Index<MaterialT> material);
        void Remove(Arca::Index<MaterialT> material);

        [[nodiscard]] Group* For(Arca::RelicID id);

        [[nodiscard]] vk::PipelineLayout PipelineLayout() const;
    private:
        std::unordered_map<Arca::RelicID, Group> groups;
    private:
        std::shared_ptr<vk::Device> device;

        vk::UniquePipelineLayout layout;
        VertexInput vertexInput;
        vk::PrimitiveTopology primitiveTopology;

        vk::RenderPass renderPass;
        vk::Extent2D swapchainExtent;

        std::vector<vk::DynamicState> dynamicStates;
    private:
        [[nodiscard]] Group CreateGroup(const MaterialT& material) const;
    };

    template<class MaterialT>
    MappedConduits<MaterialT>::MappedConduits(
        std::shared_ptr<vk::Device> device,
        VertexInput vertexInput,
        vk::PrimitiveTopology primitiveTopology,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        std::vector<vk::DynamicState> dynamicStates,
        const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts)
        :
        device(device),
        vertexInput(vertexInput),
        primitiveTopology(primitiveTopology),
        renderPass(renderPass),
        swapchainExtent(swapchainExtent),
        dynamicStates(dynamicStates)
    {
        const vk::PipelineLayoutCreateInfo layoutCreateInfo({}, descriptorSetLayouts.size(), descriptorSetLayouts.data());
        layout = device->createPipelineLayoutUnique(layoutCreateInfo);
    }

    template<class MaterialT>
    void MappedConduits<MaterialT>::Add(Arca::Index<MaterialT> material)
    {
        if (groups.find(material.ID()) == groups.end())
            groups.emplace(material.ID(), CreateGroup(*material));
    }

    template<class MaterialT>
    void MappedConduits<MaterialT>::Remove(Arca::Index<MaterialT> material)
    {
        groups.erase(material.ID());
    }

    template<class MaterialT>
    auto MappedConduits<MaterialT>::For(Arca::RelicID id) -> Group*
    {
        auto found = groups.find(id);
        if (found == groups.end())
            return nullptr;

        return &found->second;
    }

    template<class MaterialT>
    vk::PipelineLayout MappedConduits<MaterialT>::PipelineLayout() const
    {
        return layout.get();
    }

    template<class MaterialT>
    auto MappedConduits<MaterialT>::CreateGroup(const MaterialT& material) const -> Group
    {
        const auto& passes = material.Passes();

        Group group;
        group.reserve(passes.size());
        for (auto& pass : passes)
        {
            try
            {
                group.push_back(Conduit(
                    pass.VertexShader(),
                    pass.FragmentShader(),
                    *device,
                    layout.get(),
                    renderPass,
                    vertexInput,
                    swapchainExtent,
                    primitiveTopology,
                    dynamicStates));
            }
            catch (const InvalidConduit&)
            {
            }
        }
        return group;
    }
}