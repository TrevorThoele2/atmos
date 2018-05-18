#pragma once

#include <map>

#include "Entity.h"
#include "ComponentTypeName.h"

namespace Atmos
{
    namespace Ent
    {
        class ComponentBase
        {
        private:
            template<class ComponentT, class UnderlyingContainer>
            friend class ComponentMapBase;

            Entity ownerEntity;
            virtual void PostLoad() {}
        protected:
            ComponentBase();
            ComponentBase(const ComponentBase &arg);
            ComponentBase& operator=(const ComponentBase &arg);
            ComponentBase(ComponentBase &&arg);
            ComponentBase& operator=(ComponentBase &&arg);
        public:
            typedef ComponentTypeName TypeNameT;

            virtual ~ComponentBase() = 0 {}
            virtual const TypeNameT& GetTypeName() const = 0;
            Entity GetOwnerEntity() const;
        };
    }
}