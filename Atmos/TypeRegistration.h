#pragma once

#include <memory>
#include <vector>
#include <typeindex>

#include "ObjectManager.h"
#include "ObjectTypeGraph.h"

#include <Inscription/BinaryArchive.h>

namespace Atmos
{
    class TypeRegistration
    {
    public:
        TypeRegistration();

        void PushAllTo(ObjectManager& manager);
        void PushAllTo(ObjectTypeGraph& graph);
        void PushAllTo(::Inscription::BinaryArchive& archive);
    private:
        class Entry
        {
        public:
            virtual ~Entry() = 0;
            virtual void PushTo(ObjectManager& manager) = 0;
            virtual void PushTo(ObjectTypeGraph& graph) = 0;
            virtual void PushTo(::Inscription::BinaryArchive& archive) = 0;

            virtual std::type_index Type() const = 0;
        };
    public:
        class Group
        {
        public:
            template<class T, std::enable_if_t<std::is_base_of_v<Object, T>, int> = 0>
            void Register();
            template<class T, std::enable_if_t<std::is_base_of_v<ObjectSystem, T>, int> = 0>
            void Register();
            template<class T>
            void RegisterInfrastructure();

            void PushTo(ObjectManager& manager);
            void PushTo(ObjectTypeGraph& graph);
            void PushTo(::Inscription::BinaryArchive& archive);
        private:
            typedef std::unique_ptr<Entry> EntryPtr;
            typedef std::vector<EntryPtr> EntryList;
            EntryList entryList;

            template<class T, template<class> class EntryT>
            void RegisterCommon();
            template<class T>
            bool ListHas() const;
        private:
            template<class T>
            inline void VerifyRegistratableType()
            {
                static_assert(!std::is_reference_v<T>, "Registered types should not be references.");
                static_assert(!std::is_pointer_v<T>, "Registered types should not be pointers.");
                static_assert(!std::is_const_v<T>, "Registered types should not be const.");
            }
        };

        Group* CreateGroup();
    private:
        template<class T>
        class ObjectEntry : public Entry
        {
        public:
            void PushTo(ObjectManager& manager) override;
            void PushTo(ObjectTypeGraph& graph) override;
            void PushTo(::Inscription::BinaryArchive& archive) override;

            std::type_index Type() const override;
        };

        template<class T>
        class SystemEntry : public Entry
        {
        public:
            void PushTo(ObjectManager& manager) override;
            void PushTo(ObjectTypeGraph& graph) override;
            void PushTo(::Inscription::BinaryArchive& archive) override;

            std::type_index Type() const override;
        };

        template<class T>
        class InfrastructureEntry : public Entry
        {
        public:
            void PushTo(ObjectManager& manager) override;
            void PushTo(ObjectTypeGraph& graph) override;
            void PushTo(::Inscription::BinaryArchive& archive) override;

            std::type_index Type() const override;
        };
    private:
        typedef std::vector<Group> GroupList;
        GroupList groups;
    };

    template<class T, std::enable_if_t<std::is_base_of_v<Object, T>, int>>
    void TypeRegistration::Group::Register()
    {
        RegisterCommon<T, ObjectEntry>();
    }

    template<class T, std::enable_if_t<std::is_base_of_v<ObjectSystem, T>, int>>
    void TypeRegistration::Group::Register()
    {
        RegisterCommon<T, SystemEntry>();
    }

    template<class T>
    void TypeRegistration::Group::RegisterInfrastructure()
    {
        RegisterCommon<T, InfrastructureEntry>();
    }

    template<class T, template<class> class EntryT>
    void TypeRegistration::Group::RegisterCommon()
    {
        VerifyRegistratableType<T>();

        if (ListHas<T>())
            return;

        entryList.push_back(EntryPtr(new EntryT<T>()));
    }

    template<class T>
    bool TypeRegistration::Group::ListHas() const
    {
        std::type_index checkType(typeid(T));

        for (auto& loop : entryList)
        {
            auto loopType = loop->Type();

            if (loopType == checkType)
                return true;
        }

        return false;
    }

    template<class T>
    void TypeRegistration::ObjectEntry<T>::PushTo(ObjectManager& manager)
    {
        manager.RegisterObjectType<T>();
    }

    template<class T>
    void TypeRegistration::ObjectEntry<T>::PushTo(ObjectTypeGraph& graph)
    {
        auto description = TypeDescriptionFor<T>();
        graph.AddDescription(description);
    }

    template<class T>
    void TypeRegistration::ObjectEntry<T>::PushTo(::Inscription::BinaryArchive& archive)
    {
        archive.RegisterType<T>();
        archive.RegisterType<ObjectBatch<T>>();
        archive.RegisterType<ObjectBatchSource<T>>();
    }

    template<class T>
    std::type_index TypeRegistration::ObjectEntry<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class T>
    void TypeRegistration::SystemEntry<T>::PushTo(ObjectManager& manager)
    {
        manager.RegisterSystemType<T>();
    }

    template<class T>
    void TypeRegistration::SystemEntry<T>::PushTo(ObjectTypeGraph& graph)
    {}

    template<class T>
    void TypeRegistration::SystemEntry<T>::PushTo(::Inscription::BinaryArchive& archive)
    {}

    template<class T>
    std::type_index TypeRegistration::SystemEntry<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class T>
    void TypeRegistration::InfrastructureEntry<T>::PushTo(ObjectManager& manager)
    {}

    template<class T>
    void TypeRegistration::InfrastructureEntry<T>::PushTo(ObjectTypeGraph& graph)
    {}

    template<class T>
    void TypeRegistration::InfrastructureEntry<T>::PushTo(::Inscription::BinaryArchive& archive)
    {
        archive.RegisterType<T>();
    }

    template<class T>
    std::type_index TypeRegistration::InfrastructureEntry<T>::Type() const
    {
        return std::type_index(typeid(T));
    }
}