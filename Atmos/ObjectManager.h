#pragma once

#include <unordered_map>
#include <map>

#include "Object.h"
#include "ObjectTraits.h"
#include "ObjectTypeGraph.h"
#include "ObjectReference.h"
#include "ObjectFactory.h"
#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManagerException.h"

#include "IDManager.h"

#include "Event.h"

#include "IsSameRuntimeType.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectManager
    {
    public:
        typedef size_t SizeT;
    public:
        Event<ObjectReference> onAdded;
        Event<ObjectReference> onBeforeDestroyed;
    public:
        ObjectManager();
        ObjectManager(ObjectManager&& arg);
        ObjectManager& operator=(ObjectManager&& arg);

        void Initialize();
        void Work();

        template<class T, class... Args>
        TypedObjectReference<T> CreateObject(Args&& ... args);

        ObjectReference FindObject(ObjectID id);
        template<class T>
        TypedObjectReference<T> FindObject(ObjectID id);

        void DestroyObject(ObjectID id);
        void DestroyObject(ObjectReference reference);

        SizeT Size() const;

        template<class T>
        ObjectBatch<T> Batch();

        template<class SystemT>
        SystemT* FindSystem();
        template<class SystemT>
        const SystemT* FindSystem() const;

        ObjectTypeGraph TypeGraph() const;

        template<class T>
        void RegisterObjectType();
        template<class T, class... Args>
        void RegisterSystemType(Args&& ... args);
        template<class T, class System, typename std::enable_if<std::is_base_of_v<ObjectSystem, System>, int>::type = 0>
        void RegisterSystemType(System* use);
    public:
        ObjectTypeDescriptionGroup DescriptionGroupFor(const ObjectTypeName& typeName);
    private:
        ObjectTypeGraph typeGraph;
    private:
        void NotifyCreation(Object& object);
        void NotifyDestruction(Object& object);
    private:
        typedef std::unique_ptr<Object> ObjectPtr;
        typedef IDManager<ObjectID, ObjectPtr> ObjectList;

        ObjectList objects;

        template<class T>
        TypedObjectReference<T> AddAndSetupObject(T& add);
    private:
        typedef std::unique_ptr<ObjectFactoryBase> ObjectFactoryPtr;
        typedef std::unordered_map<ObjectTypeName, ObjectFactoryPtr> ObjectFactoryList;

        ObjectFactoryList factories;

        template<class Factory>
        Factory* CreateObjectFactory();
        template<class Factory>
        Factory* FindObjectFactory();
    private:
        class ObjectSystemHandle
        {
        public:
            virtual ~ObjectSystemHandle() = 0;
            virtual ObjectSystem* Get() = 0;
        };

        class OwnedObjectSystemHandle : public ObjectSystemHandle
        {
        public:
            std::unique_ptr<ObjectSystem> ptr;
            ObjectSystem* Get() override;
        };

        class UnownedObjectSystemHandle : public ObjectSystemHandle
        {
        public:
            ObjectSystem* ptr;
            ObjectSystem* Get() override;
        };

        typedef std::unique_ptr<ObjectSystemHandle> ObjectSystemHandlePtr;
        typedef std::multimap<ObjectSystemPriority, ObjectSystemHandlePtr> ObjectSystemList;

        ObjectSystemList systems;

        template<class System, class... Args>
        System* CreateObjectSystem(Args&& ... args);
        template<class System>
        System* StoreOwnedObjectSystem(System* store);
        template<class System>
        System* StoreUnownedObjectSystem(System* store);
        template<class System>
        System* FindObjectSystem();
        template<class System>
        bool HasObjectSystem() const;
    private:
        typedef std::unique_ptr<ObjectBatchSourceBase> ObjectBatchSourcePtr;
        typedef std::unordered_map<ObjectTypeName, ObjectBatchSourcePtr> ObjectBatchSourceList;

        ObjectBatchSourceList batchSources;

        // Will return an already created source if it exists
        template<class T>
        ObjectBatchSource<T>* CreateBatchSource();
        ObjectBatchSourceBase* FindBatchSource(const ObjectTypeName& typeName);
        template<class T>
        ObjectBatchSource<T>* FindBatchSource();
        template<class T>
        void DestroyBatchSource();
        void NotifyBatchSourcesCreation(Object& object);
        void NotifyBatchSourcesDestruction(Object& object);
    private:
        template<class T>
        friend class ObjectBatchSource;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class T, class... Args>
    TypedObjectReference<T> ObjectManager::CreateObject(Args&& ... args)
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        auto factory = FindObjectFactory<ObjectFactory<T>>();
        if (!factory)
            return TypedObjectReference<T>();

        auto madeObject = factory->CreateObject(std::forward<Args>(args)...);
        if (!madeObject)
            return TypedObjectReference<T>();

        return AddAndSetupObject(*madeObject);
    }

    template<class T>
    TypedObjectReference<T> ObjectManager::FindObject(ObjectID id)
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        auto found = FindObject(id);
        return TypedObjectReference<T>(static_cast<T*>(found));
    }

    template<class T>
    ObjectBatch<T> ObjectManager::Batch()
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        auto source = CreateBatchSource<T>();
        return ObjectBatch<T>(*source);
    }

    template<class SystemT>
    SystemT* ObjectManager::FindSystem()
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(SystemT);

        for (auto& loop : systems)
        {
            auto casted = dynamic_cast<SystemT*>(loop.second->Get());
            if (casted)
                return casted;
        }

        return nullptr;
    }

    template<class SystemT>
    const SystemT* ObjectManager::FindSystem() const
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(SystemT);

        for (auto& loop : systems)
        {
            auto casted = dynamic_cast<const SystemT*>(loop.second->Get());
            if (casted)
                return casted;
        }

        return nullptr;
    }

    template<class T>
    void ObjectManager::RegisterObjectType()
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        typeGraph.AddDescription(TypeDescriptionFor<T>());
        CreateObjectFactory<ObjectFactory<T>>();
    }

    template<class T, class... Args>
    void ObjectManager::RegisterSystemType(Args&& ... args)
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(T);

        auto system = CreateObjectSystem<T>(std::forward<Args>(args)...);
        StoreOwnedObjectSystem(system);
    }

    template<class T, class System, typename std::enable_if<std::is_base_of_v<ObjectSystem, System>, int>::type>
    void ObjectManager::RegisterSystemType(System* use)
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(T);

        if (!IsSameRuntimeType<T>(use))
            throw IncorrectRegisteredObjectSystemTypeException();

        StoreUnownedObjectSystem(use);
    }

    template<class T>
    TypedObjectReference<T> ObjectManager::AddAndSetupObject(T& add)
    {
        typedef TypedObjectReference<T> ReferenceT;

        auto emplaced = objects.Add(ObjectPtr(&add));

        NotifyCreation(add);
        NotifyBatchSourcesCreation(add);

        return ReferenceT(add);
    }

    template<class Factory>
    Factory* ObjectManager::CreateObjectFactory()
    {
        typedef typename Factory::Created Created;

        auto found = FindObjectFactory<Factory>();
        if (found)
            return found;

        auto objectTypeName = TypeNameFor<Created>();

        auto made = new Factory(*this);
        factories.emplace(objectTypeName, ObjectFactoryPtr(made));
        return made;
    }

    template<class Factory>
    Factory* ObjectManager::FindObjectFactory()
    {
        typedef typename Factory::Created Created;
        auto objectTypeName = TypeNameFor<Created>();

        auto found = factories.find(objectTypeName);
        if (found == factories.end())
            return nullptr;

        return static_cast<Factory*>(found->second.get());
    }

    template<class System, class... Args>
    System* ObjectManager::CreateObjectSystem(Args&& ... args)
    {
        auto found = FindObjectSystem<System>();
        if (found)
            return found;

        return new System(*this, std::forward<Args>(args)...);
    }

    template<class System>
    System* ObjectManager::StoreOwnedObjectSystem(System* store)
    {
        auto handle = new OwnedObjectSystemHandle();
        handle->ptr.reset(store);
        systems.emplace(PriorityFor<System>(), ObjectSystemHandlePtr(handle));
        return store;
    }

    template<class System>
    System* ObjectManager::StoreUnownedObjectSystem(System* store)
    {
        auto handle = new UnownedObjectSystemHandle();
        handle->ptr = store;
        systems.emplace(PriorityFor<System>(), ObjectSystemHandlePtr(handle));
        return store;
    }

    template<class System>
    System* ObjectManager::FindObjectSystem()
    {
        for (auto& loop : systems)
        {
            auto casted = dynamic_cast<System*>(loop.second.get());
            if (casted != nullptr)
                return casted;
        }

        return nullptr;
    }

    template<class System>
    bool ObjectManager::HasObjectSystem() const
    {
        return FindObjectSystem<System>() != nullptr;
    }

    template<class T>
    ObjectBatchSource<T>* ObjectManager::CreateBatchSource()
    {
        auto found = FindBatchSource<T>();
        if (found)
            return found;

        auto typeName = TypeNameFor<T>();

        auto created = new ObjectBatchSource<T>(*this);
        batchSources.emplace(typeName, ObjectBatchSourcePtr(created));
        return created;
    }

    template<class T>
    ObjectBatchSource<T>* ObjectManager::FindBatchSource()
    {
        auto found = batchSources.find(TypeNameFor<T>());
        if (found == batchSources.end())
            return nullptr;

        return static_cast<ObjectBatchSource<T>*>(found->second.get());
    }

    template<class T>
    void ObjectManager::DestroyBatchSource()
    {
        batchSources.erase(TypeNameFor<T>());
    }
}

#include "ObjectBatchSourceDefinition.h"