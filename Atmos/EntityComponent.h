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

            const TypeNameT& GetTypeName() const override;
            static const TypeNameT& TypeName() { return Map::TypeName(); }
        };

        template<class Mixin>
        typename const Component<Mixin>::TypeNameT& Component<Mixin>::GetTypeName() const
        {
            return TypeName();
        }
    }
}