
#include "ObjectManagerFactory.h"

namespace Atmos
{
    ObjectManager ObjectManagerFactory::Produce() const
    {
        ObjectManager objectManager;
        for (auto& loop : typeRegistratorList)
            loop->Register(objectManager);
        objectManager.Initialize();
        return objectManager;
    }

    ObjectManagerFactory::TypeRegistrator::~TypeRegistrator()
    {}
}