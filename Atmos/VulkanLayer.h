#pragma once

#include <unordered_map>
#include "MaterialAsset.h"

namespace Atmos::Render::Vulkan
{
    template<class MaterialGroupKey, class MaterialGroupValue>
    struct Layer
    {
        struct MaterialGroup
        {
            std::unordered_map<MaterialGroupKey, std::vector<MaterialGroupValue>> values;
            std::vector<MaterialGroupValue>& ListFor(const MaterialGroupKey& key);
        };

        std::unordered_map<const Asset::Material*, MaterialGroup> materialGroups;
        MaterialGroup& GroupFor(const Asset::Material& material);
    };

    template<class MaterialGroupKey, class MaterialGroupValue>
    std::vector<MaterialGroupValue>& Layer<MaterialGroupKey, MaterialGroupValue>::MaterialGroup::ListFor(
        const MaterialGroupKey& key)
    {
        auto found = values.find(key);
        return found == values.end() ? values.emplace(key, std::vector<MaterialGroupValue>{}).first->second : found->second;
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto Layer<MaterialGroupKey, MaterialGroupValue>::GroupFor(const Asset::Material& material) -> MaterialGroup&
    {
        auto found = materialGroups.find(&material);
        return found == materialGroups.end() ? materialGroups.emplace(&material, MaterialGroup{}).first->second : found->second;
    }

    template<class MaterialGroupValue>
    struct Layer<void, MaterialGroupValue>
    {
        struct MaterialGroup
        {
            std::vector<MaterialGroupValue> values;
        };

        std::unordered_map<const Asset::Material*, MaterialGroup> materialGroups;
        MaterialGroup& GroupFor(const Asset::Material& material);
    };

    template<class MaterialGroupValue>
    auto Layer<void, MaterialGroupValue>::GroupFor(const Asset::Material& material) -> MaterialGroup&
    {
        auto found = materialGroups.find(&material);
        return found == materialGroups.end() ? materialGroups.emplace(&material, MaterialGroup{}).first->second : found->second;
    }
}