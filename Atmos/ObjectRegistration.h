#pragma once

#include <memory>
#include <vector>
#include <typeinfo>

#include "ObjectManager.h"
#include "ObjectTypeGraph.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    class ObjectRegistration
    {
    public:
        ObjectRegistration();
    public:
        template<class T, std::enable_if_t<std::is_base_of_v<Object, T>, int> = 0>
        void RegisterGlobal();
        template<class T, std::enable_if_t<std::is_base_of_v<ObjectSystem, T>, int> = 0>
        void RegisterGlobal();
        template<class T, std::enable_if_t<std::is_base_of_v<Object, T>, int> = 0>
        void RegisterLocal();
        template<class T, std::enable_if_t<std::is_base_of_v<ObjectSystem, T>, int> = 0>
        void RegisterLocal();
        template<class T>
        void RegisterInfrastructure();
    public:
        void RegisterDefaultGlobals();
        void RegisterDefaultLocals();
        void RegisterDefaultInfrastructures();

        void RegisterAllDefaults();
    public:
        void PushGlobalsTo(ObjectManager& manager);
        void PushLocalsTo(ObjectManager& manager);

        void PushTo(ObjectTypeGraph& graph);

        void PushTo(::Inscription::BinaryScribe& scribe);
    private:
        template<class T>
        inline void VerifyRegistratableType()
        {
            static_assert(!std::is_reference_v<T>, "Registered types should not be references.");
            static_assert(!std::is_pointer_v<T>, "Registered types should not be pointers.");
            static_assert(!std::is_const_v<T>, "Registered types should not be const.");
        }
    private:
        class Entry
        {
        public:
            virtual ~Entry() = 0;
            virtual void PushTo(ObjectManager& manager) = 0;
            virtual void PushTo(ObjectTypeGraph& graph) = 0;
            virtual void PushTo(::Inscription::BinaryScribe& scribe) = 0;
        };

        template<class T>
        class ObjectEntry : public Entry
        {
        public:
            void PushTo(ObjectManager& manager) override;
            void PushTo(ObjectTypeGraph& graph) override;
            void PushTo(::Inscription::BinaryScribe& scribe) override;
        };

        template<class T>
        class SystemEntry : public Entry
        {
        public:
            void PushTo(ObjectManager& manager) override;
            void PushTo(ObjectTypeGraph& graph) override;
            void PushTo(::Inscription::BinaryScribe& scribe) override;
        };

        template<class T>
        class InfrastructureEntry : public Entry
        {
        public:
            void PushTo(ObjectManager& manager) override;
            void PushTo(ObjectTypeGraph& graph) override;
            void PushTo(::Inscription::BinaryScribe& scribe) override;
        };

        typedef std::unique_ptr<Entry> EntryPtr;
        typedef std::vector<EntryPtr> EntryList;
        EntryList globalEntryList;
        EntryList localEntryList;
        EntryList infrastructureEntryList;

        template<class T, template<class> class EntryT>
        void RegisterCommon(EntryList& list);
        template<class T>
        bool ListHas(const EntryList& list) const;
    };

    template<class T, std::enable_if_t<std::is_base_of_v<Object, T>, int>>
    void ObjectRegistration::RegisterGlobal()
    {
        RegisterCommon<T, ObjectEntry>(globalEntryList);
    }

    template<class T, std::enable_if_t<std::is_base_of_v<ObjectSystem, T>, int>>
    void ObjectRegistration::RegisterGlobal()
    {
        RegisterCommon<T, SystemEntry>(globalEntryList);
    }

    template<class T, std::enable_if_t<std::is_base_of_v<Object, T>, int>>
    void ObjectRegistration::RegisterLocal()
    {
        RegisterCommon<T, ObjectEntry>(localEntryList);
    }

    template<class T, std::enable_if_t<std::is_base_of_v<ObjectSystem, T>, int>>
    void ObjectRegistration::RegisterLocal()
    {
        RegisterCommon<T, SystemEntry>(globalEntryList);
    }

    template<class T>
    void ObjectRegistration::RegisterInfrastructure()
    {
        RegisterCommon<T, InfrastructureEntry>(infrastructureEntryList);
    }

    template<class T>
    void ObjectRegistration::ObjectEntry<T>::PushTo(ObjectManager& manager)
    {
        manager.RegisterObjectType<T>();
    }

    template<class T>
    void ObjectRegistration::ObjectEntry<T>::PushTo(ObjectTypeGraph& graph)
    {
        auto description = TypeDescriptionFor<T>();
        graph.AddDescription(description);
    }

    template<class T>
    void ObjectRegistration::ObjectEntry<T>::PushTo(::Inscription::BinaryScribe& scribe)
    {
        scribe.RegisterType<T>();
        scribe.RegisterType<ObjectBatch<T>>();
    }

    template<class T>
    void ObjectRegistration::SystemEntry<T>::PushTo(ObjectManager& manager)
    {
        manager.RegisterSystemType<T>();
    }

    template<class T>
    void ObjectRegistration::SystemEntry<T>::PushTo(ObjectTypeGraph& graph)
    {}

    template<class T>
    void ObjectRegistration::SystemEntry<T>::PushTo(::Inscription::BinaryScribe& scribe)
    {
        scribe.RegisterType<T>();
    }

    template<class T>
    void ObjectRegistration::InfrastructureEntry<T>::PushTo(ObjectManager& manager)
    {}

    template<class T>
    void ObjectRegistration::InfrastructureEntry<T>::PushTo(ObjectTypeGraph& graph)
    {}

    template<class T>
    void ObjectRegistration::InfrastructureEntry<T>::PushTo(::Inscription::BinaryScribe& scribe)
    {
        scribe.RegisterType<T>();
    }

    template<class T, template<class> class EntryT>
    void ObjectRegistration::RegisterCommon(EntryList& list)
    {
        VerifyRegistratableType<T>();

        if (ListHas<T>(list))
            return;

        list.push_back(EntryPtr(new EntryT<T>()));
    }

    template<class T>
    bool ObjectRegistration::ListHas(const EntryList& list) const
    {
        auto& checkType = typeid(T);

        for (auto& loop : list)
        {
            auto& loopType = typeid(T);

            if (loopType == checkType)
                return true;
        }

        return false;
    }
}