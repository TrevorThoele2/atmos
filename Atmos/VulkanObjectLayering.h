#pragma once

#include <map>

#include "VulkanLayer.h"
#include "Point3D.h"

namespace Atmos::Render::Vulkan
{
    struct ObjectLayeringKey
    {
        int space;
        Spatial::Point3D::Value z;

        ObjectLayeringKey() = default;
        ObjectLayeringKey(int space, Spatial::Point3D::Value z);

        bool operator==(const ObjectLayeringKey& arg) const;
        bool operator!=(const ObjectLayeringKey& arg) const;
        bool operator<(const ObjectLayeringKey& arg) const;
    };

    template<class MaterialGroupKey, class MaterialGroupValue>
    class ObjectLayering
    {
    public:
        using Layer = Layer<MaterialGroupKey, MaterialGroupValue>;
        using Key = ObjectLayeringKey;
    private:
        using Layers = std::map<Key, Layer>;
    public:
        using iterator = typename Layers::iterator;
        using const_iterator = typename Layers::const_iterator;
    public:
        Layer& Add(Key key, Layer&& value);
        Layer* Find(Key key);
        void Clear();

        [[nodiscard]] uint32_t Count() const;
        [[nodiscard]] bool Empty() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        Layers layers;
    };

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto ObjectLayering<MaterialGroupKey, MaterialGroupValue>::Add(Key key, Layer&& value) -> Layer&
    {
        return layers.emplace(key, std::move(value)).first->second;
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto ObjectLayering<MaterialGroupKey, MaterialGroupValue>::Find(Key key) -> Layer*
    {
        auto found = layers.find(key);
        return found != layers.end() ? &found->second : nullptr;
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    void ObjectLayering<MaterialGroupKey, MaterialGroupValue>::Clear()
    {
        layers.clear();
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    uint32_t ObjectLayering<MaterialGroupKey, MaterialGroupValue>::Count() const
    {
        return layers.size();
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    bool ObjectLayering<MaterialGroupKey, MaterialGroupValue>::Empty() const
    {
        return layers.empty();
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto ObjectLayering<MaterialGroupKey, MaterialGroupValue>::begin() -> iterator
    {
        return layers.begin();
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto ObjectLayering<MaterialGroupKey, MaterialGroupValue>::begin() const -> const_iterator
    {
        return layers.begin();
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto ObjectLayering<MaterialGroupKey, MaterialGroupValue>::end() -> iterator
    {
        return layers.end();
    }

    template<class MaterialGroupKey, class MaterialGroupValue>
    auto ObjectLayering<MaterialGroupKey, MaterialGroupValue>::end() const -> const_iterator
    {
        return layers.end();
    }
}