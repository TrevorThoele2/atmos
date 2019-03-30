#pragma once

#include "ObjectID.h"
#include "ObjectTypeDescription.h"
#include "ObjectTraits.h"
#include "ObjectStaticAssert.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectManager;
    class Object
    {
    public:
        Object();
        Object(const ::Inscription::Table<Object>& table);
        virtual ~Object() = 0;

        ObjectID ID() const;
        ObjectManager* Manager();
        const ObjectManager* Manager() const;

        template<class T>
        bool Is() const;
        template<class T>
        T* As();
        template<class T>
        const T* As() const;

        virtual ObjectTypeDescription TypeDescription() const = 0;
    private:
        ObjectID id;
        ObjectManager* manager;
    private:
        friend ObjectManager;
    public:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    bool Object::Is() const
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        auto description = TypeDescription();
        return ObjectTraits<T>::TypeDescription() == description;
    }

    template<class T>
    T* Object::As()
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        if (!Is<T>())
            return nullptr;

        return static_cast<T*>(this);
    }

    template<class T>
    const T* Object::As() const
    {
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);

        if (!Is<T>())
            return nullptr;

        return static_cast<const T*>(this);
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Object)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}