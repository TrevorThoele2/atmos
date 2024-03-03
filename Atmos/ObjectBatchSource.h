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
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.manager);
            if (archive.IsInput())
                object.referenceCount = 0;
        }

        static void Construct(ObjectT*& object, ArchiveT& archive)
        {
            ::Atmos::ObjectManager* manager;
            archive(manager);

            object = new ObjectT(*manager);

            if (archive.IsInput())
                object->referenceCount = 0;
        }

        static const ClassNameResolver classNameResolver;
    };
    
    template<class T>
    typename const Scribe<::Atmos::ObjectBatchSource<T>, BinaryArchive>::ClassNameResolver
        Scribe<::Atmos::ObjectBatchSource<T>, BinaryArchive>::classNameResolver = ClassNameResolver([](ArchiveT& archive) -> ClassName
    {
        ClassName baseName("ObjectBatchSource");
        ClassName objectName(::Atmos::TypeNameFor<T>());
        return baseName + objectName;
    });
}