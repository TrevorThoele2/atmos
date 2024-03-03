#pragma once

#include <memory>
#include <vector>

#include "ObjectManager.h"
#include "ObjectTypeGraph.h"

namespace Atmos
{
    class ObjectManagerFactory
    {
    public:
        template<class ObjectT, typename std::enable_if<std::is_base_of<Object, ObjectT>::value, int>::type = 0>
        void RegisterType();
        template<class SystemT, typename std::enable_if<std::is_base_of<ObjectSystem, SystemT>::value, int>::type = 0>
        void RegisterType();

        ObjectManager Produce() const;
    private:
        class TypeRegistrator
        {
        public:
            virtual ~TypeRegistrator() = 0;

            virtual void Register(ObjectManager& manager) = 0;
            virtual void ToTypeGraph(ObjectTypeGraph& typeGraph) = 0;
        };

        template<class T>
        class ObjectTypeRegistrator : public TypeRegistrator
        {
        public:
            void RegisterInto(ObjectManager& manager) override;
            void ToTypeGraph(ObjectTypeGraph& typeGraph) override;
        };

        template<class T>
        class SystemTypeRegistrator : public TypeRegistrator
        {
        public:
            void RegisterInto(ObjectManager& manager) override;
            void ToTypeGraph(ObjectTypeGraph& typeGraph) override;
        };

        typedef std::unique_ptr<TypeRegistrator> TypeRegistratorPtr;
        typedef std::vector<TypeRegistratorPtr> TypeRegistratorList;

        TypeRegistratorList typeRegistratorList;
    };

    template<class ObjectT, typename std::enable_if<std::is_base_of<Object, ObjectT>::value, int>::type>
    void ObjectManagerFactory::RegisterType()
    {
        typeRegistratorList.push_back(std::make_unique<ObjectTypeRegistrator<ObjectT>>());
    }

    template<class SystemT, typename std::enable_if<std::is_base_of<ObjectSystem, SystemT>::value, int>::type>
    void ObjectManagerFactory::RegisterType()
    {
        typeRegistratorList.push_back(std::make_unique<SystemTypeRegistrator<SystemT>>());
    }

    template<class T>
    void ObjectManagerFactory::ObjectTypeRegistrator<T>::RegisterInto(ObjectManager& manager)
    {
        manager.RegisterObjectType<T>();
    }

    template<class T>
    void ObjectManagerFactory::ObjectTypeRegistrator<T>::ToTypeGraph(ObjectTypeGraph& typeGraph)
    {
        typeGraph.AddDescription(TypeDescriptionFor<T>());
    }

    template<class T>
    void ObjectManagerFactory::SystemTypeRegistrator<T>::RegisterInto(ObjectManager& manager)
    {
        manager.RegisterObjectSystem<T>();
    }

    template<class T>
    void ObjectManagerFactory::SystemTypeRegistrator<T>::ToTypeGraph(ObjectTypeGraph& typeGraph)
    {}
}