#pragma once

#include <unordered_map>
#include <Arca/RelicID.h>

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

        std::unordered_map<Arca::RelicID, MaterialGroup> materialGroups;
        MaterialGroup& GroupFor(Arca::RelicID id);
    };

    template<class MaterialGroupKey, class MaterialGroupValue>
    std::vector<MaterialGroupValue>& Layer<MaterialGroupKey, MaterialGroupValue>::MaterialGroup::ListFor(
        const MaterialGroupKey& key)
    {
        auto found = values.find(key);
        return found == values.end() ? values.emplace(key, std::vector<MaterialGroupValue>{}).first->second : found->second;
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto Layer<MaterialGroupKey, MaterialGroupValue>::GroupFor(Arca::RelicID id) -> MaterialGroup&
    {
        auto found = materialGroups.find(id);
        return found == materialGroups.end() ? materialGroups.emplace(id, MaterialGroup{}).first->second : found->second;
    }

    template<class MaterialGroupValue>
    struct Layer<void, MaterialGroupValue>
    {
        struct MaterialGroup
        {
            std::vector<MaterialGroupValue> values;
        };

        std::unordered_map<Arca::RelicID, MaterialGroup> materialGroups;
        MaterialGroup& GroupFor(Arca::RelicID id);
    };

    template<class MaterialGroupValue>
    auto Layer<void, MaterialGroupValue>::GroupFor(Arca::RelicID id) -> MaterialGroup&
    {
        auto found = materialGroups.find(id);
        return found == materialGroups.end() ? materialGroups.emplace(id, MaterialGroup{}).first->second : found->second;
    }
}