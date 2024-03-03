#pragma once

#include "ObjectFactoryBase.h"

namespace Atmos
{
    template<class T>
    class ObjectFactory : public ObjectFactoryBase
    {
    public:
        typedef T Created;
    public:
        ObjectFactory(ObjectManager& manager);

        template<class... Args>
        Created* CreateObject(Args && ... args);
    };

    template<class T>
    ObjectFactory<T>::ObjectFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    template<class T>
    template<class... Args>
    typename ObjectFactory<T>::Created* ObjectFactory<T>::CreateObject(Args && ... args)
    {
        return new Created(std::forward<Args>(args)...);
    }
}