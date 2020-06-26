#pragma once

#include <map>
#include "Position3D.h"

namespace Atmos::Render::Vulkan
{
    template<class Context>
    class ObjectLayering
    {
    private:
        using LayeredContexts = std::map<Position3D::Value, Context>;
    public:
        using iterator = typename LayeredContexts::iterator;
        using const_iterator = typename LayeredContexts::const_iterator;
    public:
        Context& Add(Position3D::Value key, Context&& value);
        Context* Find(Position3D::Value value);
        void Clear();

        [[nodiscard]] uint32_t Count() const;
        [[nodiscard]] bool Empty() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        LayeredContexts layeredContexts;
    };

    template<class Context>
    Context& ObjectLayering<Context>::Add(Position3D::Value key, Context&& value)
    {
        return layeredContexts.emplace(key, std::move(value)).first->second;
    }

    template<class Context>
    auto ObjectLayering<Context>::Find(Position3D::Value value) -> Context*
    {
        auto found = layeredContexts.find(value);
        if (found == layeredContexts.end())
            return nullptr;

        return &found->second;
    }

    template<class Context>
    void ObjectLayering<Context>::Clear()
    {
        layeredContexts.clear();
    }

    template<class Context>
    uint32_t ObjectLayering<Context>::Count() const
    {
        return layeredContexts.size();
    }

    template<class Context>
    bool ObjectLayering<Context>::Empty() const
    {
        return layeredContexts.empty();
    }

    template<class Context>
    auto ObjectLayering<Context>::begin() -> iterator
    {
        return layeredContexts.begin();
    }

    template<class Context>
    auto ObjectLayering<Context>::begin() const -> const_iterator
    {
        return layeredContexts.begin();
    }

    template<class Context>
    auto ObjectLayering<Context>::end() -> iterator
    {
        return layeredContexts.end();
    }

    template<class Context>
    auto ObjectLayering<Context>::end() const -> const_iterator
    {
        return layeredContexts.end();
    }
}