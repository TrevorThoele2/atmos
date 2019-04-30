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
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ObjectBatchSourceBase)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
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
        NullEvent onInvalidated;

        Event<Reference> onCreated;

        Event<Reference> onBeforeDestroyed;
    public:
        ObjectBatchSource(ObjectManager& manager);
        ObjectBatchSource(const ::Inscription::Table<ObjectBatchSource>& table);
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
    class Inscripter<::Atmos::ObjectBatchSource<T>> : public InscripterBase<::Atmos::ObjectBatchSource<T>>
    {
    public:
        INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(::Atmos::ObjectBatchSource<T>);
    public:
        INSCRIPTION_INSCRIPTER_TABLE
        {
            INSCRIPTION_INSCRIPTER_CREATE_TABLE

            INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectBatchSourceBase)

            INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(manager)

            INSCRIPTION_INSCRIPTER_RETURN_TABLE
        };

        INSCRIPTION_INSCRIPTER_SERIALIZE_FUNCTION
        {
            if (scribe.IsInput())
                obj.referenceCount = 0;
            INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;
        };

        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };

    template<class T>
    ::Inscription::ClassNameResolver Inscripter<::Atmos::ObjectBatchSource<T>>::classNameResolver([](Scribe& scribe) -> ClassName
    {
        ClassName baseName("ObjectBatchSource");
        ClassName objectName(::Atmos::TypeNameFor<T>());
        return baseName + objectName;
    });
}