#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <map>
#include <typeindex>

#include "Entity.h"
#include "ComponentTypeName.h"

namespace Atmos
{
    namespace Ent
    {
        class ComponentBase;
        class ComponentFactoryHandler
        {
        private:
            class FactoryBase
            {
            public:
                virtual ~FactoryBase() = 0 {}
                virtual ComponentBase* Make(Entity entity) const = 0;
            };
        public:
            template<class CompT>
            class Factory : public FactoryBase
            {
            public:
                Factory();
                CompT* Make(Entity entity) const override;
            };
        private:
            typedef std::unique_ptr<FactoryBase> FactoryPtr;
            std::unordered_map<ComponentTypeName, FactoryPtr> map;

            ComponentFactoryHandler() = default;
            ComponentFactoryHandler(const ComponentFactoryHandler &arg) = delete;
            ComponentFactoryHandler& operator=(const ComponentFactoryHandler &arg) = delete;
        public:
            static ComponentFactoryHandler& Instance();
            static void Add(const ComponentTypeName &type, FactoryBase *factory);
            static ComponentBase* Make(Entity entity, const ComponentTypeName &type);
        };

        template<class CompT>
        ComponentFactoryHandler::Factory<CompT>::Factory()
        {
            ComponentFactoryHandler::Add(CompT::TypeName(), this);
        }

        template<class CompT>
        CompT* ComponentFactoryHandler::Factory<CompT>::Make(Entity entity) const
        {
            CreateComponent<CompT>(entity);
        }
    }
}