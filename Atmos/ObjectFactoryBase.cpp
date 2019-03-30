
#include "ObjectFactoryBase.h"

namespace Atmos
{
    ObjectFactoryBase::ObjectFactoryBase(ObjectManager& manager) : manager(&manager)
    {}

    ObjectFactoryBase::~ObjectFactoryBase()
    {}

    ObjectManager* ObjectFactoryBase::Manager()
    {
        return manager;
    }
}