#pragma once

#include "EntityComponentBase.h"
#include "EntityComponentMap.h"
#include "EntityManager.h"

namespace Atmos
{
    namespace Ent
    {
        template<class Mixin>
        class Component : public ComponentBase
        {
        public:
            typedef ComponentMap<Mixin> Map;

            Component() = default;
            Component(const Component &arg) = default;
            Component(Component &&arg);
            Component& operator=(const Component &arg) = default;
            Component& operator=(Component &&arg);

            const TypeNameT& GetTypeName() const override;
            static const TypeNameT& TypeName() { return Map::TypeName(); }
        };

        template<class Mixin>
        Component<Mixin>::Component(Component &&arg) : ComponentBase(std::move(arg))
        {}

        template<class Mixin>
        Component<Mixin>& Component<Mixin>::operator=(Component &&arg)
        {
            ComponentBase::operator=(std::move(arg));
            return *this;
        }

        template<class Mixin>
        typename const Component<Mixin>::TypeNameT& Component<Mixin>::GetTypeName() const
        {
            return TypeName();
        }
    }
}