#pragma once

#include <list>

#include "ObjectID.h"
#include "Object.h"
#include "ObjectReference.h"
#include "ObjectStaticAssert.h"
#include "ObjectTraits.h"
#include "ObjectTypeName.h"
#include "ObjectTypeDescriptionGroup.h"

#include "Event.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectManager;

    class ObjectBatchSourceBase
    {
    public:
        typedef size_t SizeT;
    public:
        virtual ~ObjectBatchSourceBase() = 0;
    public:
        virtual void NotifyCreated(Object& object) = 0;
        virtual void NotifyDestroyed(Object& object) = 0;
    public:
        virtual void IncrementReference() = 0;
        virtual void DecrementReference() = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::ObjectBatchSourceBase, BinaryArchive> :
        public CompositeScribe<::Atmos::ObjectBatchSourceBase, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {}

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {}
    };
}

namespace Atmos
{
    template<class T>
    class ObjectBatchSource : public ObjectBatchSourceBase
    {
    public:
        typedef TypedObjectReference<T> Reference;
    private:
        typedef std::list<Reference> List;
    public:
        typedef typename List::size_type SizeT;
        typedef typename List::iterator iterator;
        typedef typename List::const_iterator const_iterator;
    public:
        Event<> onInvalidated;

        Event<Reference> onCreated;
        Event<Reference> onBeforeDestroyed;
    public:
        ObjectBatchSource(ObjectManager& manager);
        ~ObjectBatchSource();

        iterator RemoveObject(iterator itr);
        iterator RemoveObject(const_iterator itr);

        Reference FindObject(ObjectID id) const;

        SizeT Size() const;
        bool IsEmpty() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    public:
        void NotifyCreated(Object& object) override;
        void NotifyDestroyed(Object& object) override;
    public:
        void IncrementReference() override;
        void DecrementReference() override;
    private:
        ObjectTypeDescriptionGroup associatedDescriptionGroup;

        List list;

        ObjectManager* manager;

        size_t referenceCount;
    private:
        void SetupAssociatedDescriptionGroup();
        void Synchronize();
    private:
        bool IsManaging(const ObjectTypeName& checkAgainst) const;
        bool IsManaging(ObjectID checkAgainst) const;
        bool IsManaging(const Object& checkAgainst) const;

        static T* Downcast(Object* object);
        static T& Downcast(Object& object);
    private:
        INSCRIPTION_ACCESS;
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::ObjectBatchSource<T>, BinaryArchive> :
        public CompositeScribe<::Atmos::ObjectBatchSource<T>, BinaryArchive>
    {
    private:
        using BaseT = typename CompositeScribe<::Atmos::ObjectBatchSource<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using ClassNameResolver = typename BaseT::ClassNameResolver;

        using BaseT::Scriven;
        using BaseT::Construct;
    public:
        static const ClassNameResolver classNameResolver;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.manager);
            if (archive.IsInput())
                object.referenceCount = 0;
        }
        
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            ::Atmos::ObjectManager* manager;
            archive(manager);

            new (storage) ObjectT(*manager);

            if (archive.IsInput())
                storage->referenceCount = 0;
        }
    };
    
    template<class T>
    typename const Scribe<::Atmos::ObjectBatchSource<T>, BinaryArchive>::ClassNameResolver
        Scribe<::Atmos::ObjectBatchSource<T>, BinaryArchive>::classNameResolver =
        ClassNameResolver([](ArchiveT& archive) -> ClassName
    {
        ClassName baseName("ObjectBatchSource");
        ClassName objectName(::Atmos::TypeNameFor<T>());
        return baseName + "<" + objectName + ">";
    });
}