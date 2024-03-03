#pragma once

namespace Atmos
{
    class ObjectManager;

    class ObjectFactoryBase
    {
    public:
        ObjectFactoryBase(ObjectManager& manager);

        virtual ~ObjectFactoryBase() = 0;
    protected:
        ObjectManager* Manager();
    private:
        ObjectManager* manager;
    };
}