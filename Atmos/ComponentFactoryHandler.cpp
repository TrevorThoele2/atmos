
#include "ComponentFactoryHandler.h"

namespace Atmos
{
    namespace Ent
    {
        ComponentFactoryHandler& ComponentFactoryHandler::Instance()
        {
            static ComponentFactoryHandler instance;
            return instance;
        }

        void ComponentFactoryHandler::Add(const ComponentTypeName &type, FactoryBase *factory)
        {
            Instance().map.emplace(type, FactoryPtr(factory));
        }

        ComponentBase* ComponentFactoryHandler::Make(Entity entity, const ComponentTypeName &type)
        {
            return Instance().map.find(type)->second->Make(entity);
        }
    }
}