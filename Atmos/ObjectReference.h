#pragma once

#include "ObjectTypeName.h"
#include "ObjectID.h"
#include "Object.h"
#include "ObjectStaticAssert.h"

#include "Assert.h"

namespace Atmos
{
    class ObjectManager;

    template<class T>
    class TypedObjectReference;
    template<class T>
    class ConstTypedObjectReference;

    typedef TypedObjectReference<Object> ObjectReference;
    typedef ConstTypedObjectReference<Object> ConstObjectReference;

    template<class T>
    class TypedObjectReference
    {
    public:
        TypedObjectReference();
        TypedObjectReference(T* obj);
        TypedObjectReference(T& obj);
        template<class U = T, typename std::enable_if<!std::is_same<U, Object>::value, int>::type = 0>
        TypedObjectReference(ObjectReference& reference);
        TypedObjectReference(const TypedObjectReference& arg) = default;
        TypedObjectReference(TypedObjectReference&& arg);

        TypedObjectReference& operator=(const TypedObjectReference& arg) = default;
        TypedObjectReference& operator=(TypedObjectReference&& arg);
        TypedObjectReference& operator=(T* obj);
        TypedObjectReference& operator=(T& obj);

        bool operator==(const TypedObjectReference& arg) const;
        bool operator==(const ConstTypedObjectReference<T>& arg) const;
        bool operator!=(const TypedObjectReference& arg) const;
        bool operator!=(const ConstTypedObjectReference<T>& arg) const;

        explicit operator bool() const;

        operator ObjectReference() const;
        operator ConstTypedObjectReference<T>() const;
        template<class BaseT, typename std::enable_if<std::is_base_of<BaseT, T>::value, int>::type = 0>
        operator TypedObjectReference<BaseT>() const
        {
            return TypedObjectReference<BaseT>(obj);
        }

        T* operator->();
        const T* operator->() const;
        T& operator*();
        const T& operator*() const;

        template<class DerivedT, typename std::enable_if<std::is_base_of<T, DerivedT>::value, int>::type = 0>
        TypedObjectReference<DerivedT> Downcast() const
        {
            return TypedObjectReference<DerivedT>(dynamic_cast<DerivedT*>(obj));
        }

        ConstTypedObjectReference<T> ToConst() const;

        void Reset();

        T* Get();
        const T* Get() const;

        bool IsOccupied() const;
    private:
        T* obj;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class T>
    TypedObjectReference<T>::TypedObjectReference() : obj(nullptr)
    {}

    template<class T>
    TypedObjectReference<T>::TypedObjectReference(T* obj) : obj(obj)
    {}

    template<class T>
    TypedObjectReference<T>::TypedObjectReference(T& obj) : obj(&obj)
    {}

    template<class T>
    template<class U, typename std::enable_if<!std::is_same<U, Object>::value, int>::type>
    TypedObjectReference<T>::TypedObjectReference(ObjectReference& reference) : obj(static_cast<T*>(reference.Get()))
    {}

    template<class T>
    TypedObjectReference<T>::TypedObjectReference(TypedObjectReference&& arg) : obj(arg.obj)
    {
        arg.obj = nullptr;
    }

    template<class T>
    TypedObjectReference<T>& TypedObjectReference<T>::operator=(TypedObjectReference&& arg)
    {
        obj = arg.obj;
        arg.obj = nullptr;
        return *this;
    }

    template<class T>
    TypedObjectReference<T>& TypedObjectReference<T>::operator=(T* obj)
    {
        this->obj = obj;
        return *this;
    }

    template<class T>
    TypedObjectReference<T>& TypedObjectReference<T>::operator=(T& obj)
    {
        this->obj = &obj;
        return *this;
    }

    template<class T>
    bool TypedObjectReference<T>::operator==(const TypedObjectReference& arg) const
    {
        return obj == arg.obj;
    }

    template<class T>
    bool TypedObjectReference<T>::operator==(const ConstTypedObjectReference<T>& arg) const
    {
        return ToConst() == arg;
    }

    template<class T>
    bool TypedObjectReference<T>::operator!=(const TypedObjectReference& arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    bool TypedObjectReference<T>::operator!=(const ConstTypedObjectReference<T>& arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    TypedObjectReference<T>::operator bool() const
    {
        return IsOccupied();
    }

    template<class T>
    TypedObjectReference<T>::operator ObjectReference() const
    {
        if (!IsOccupied())
            return ObjectReference();

        return ObjectReference(obj);
    }

    template<class T>
    TypedObjectReference<T>::operator ConstTypedObjectReference<T>() const
    {
        return ToConst();
    }

    template<class T>
    T* TypedObjectReference<T>::operator->()
    {
        return Get();
    }

    template<class T>
    const T* TypedObjectReference<T>::operator->() const
    {
        return Get();
    }

    template<class T>
    T& TypedObjectReference<T>::operator*()
    {
        return *Get();
    }

    template<class T>
    const T& TypedObjectReference<T>::operator*() const
    {
        return *Get();
    }

    template<class T>
    ConstTypedObjectReference<T> TypedObjectReference<T>::ToConst() const
    {
        if (!IsOccupied())
            return ConstTypedObjectReference<T>();

        return ConstTypedObjectReference<T>(obj);
    }

    template<class T>
    void TypedObjectReference<T>::Reset()
    {
        obj = nullptr;
    }

    template<class T>
    T* TypedObjectReference<T>::Get()
    {
        return obj;
    }

    template<class T>
    const T* TypedObjectReference<T>::Get() const
    {
        return obj;
    }

    template<class T>
    bool TypedObjectReference<T>::IsOccupied() const
    {
        return Get() != nullptr;
    }

    template<class T>
    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(TypedObjectReference<T>)
    {
        scribe.UnowningPointer(obj);
    }

    template<class T>
    class ConstTypedObjectReference
    {
    public:
        ConstTypedObjectReference();
        ConstTypedObjectReference(const T* obj);
        ConstTypedObjectReference(const T& obj);
        ConstTypedObjectReference(T* obj);
        ConstTypedObjectReference(T& obj);
        template<class U = T, typename std::enable_if<!std::is_same<U, Object>::value, int>::type = 0>
        ConstTypedObjectReference(ConstObjectReference& reference);
        ConstTypedObjectReference(const ConstTypedObjectReference& arg) = default;
        ConstTypedObjectReference(ConstTypedObjectReference&& arg);

        ConstTypedObjectReference& operator=(const ConstTypedObjectReference& arg) = default;
        ConstTypedObjectReference& operator=(ConstTypedObjectReference&& arg);
        ConstTypedObjectReference& operator=(const T* obj);
        ConstTypedObjectReference& operator=(const T& obj);

        bool operator==(const ConstTypedObjectReference& arg) const;
        bool operator!=(const ConstTypedObjectReference& arg) const;

        explicit operator bool() const;

        operator ConstObjectReference() const;
        template<class BaseT, typename std::enable_if<std::is_base_of<BaseT, T>::value, int>::type = 0>
        operator ConstTypedObjectReference<BaseT>() const
        {
            return ConstTypedObjectReference<BaseT>(obj);
        }

        const T* operator->() const;
        const T& operator*() const;

        template<class DerivedT, typename std::enable_if<std::is_base_of<T, DerivedT>::value, int>::type = 0>
        ConstTypedObjectReference<DerivedT> Downcast() const
        {
            return ConstTypedObjectReference<DerivedT>(dynamic_cast<DerivedT*>(obj));
        }

        void Reset();

        const T* Get() const;

        bool IsOccupied() const;
    private:
        const T* obj;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class T>
    ConstTypedObjectReference<T>::ConstTypedObjectReference() : obj(nullptr)
    {}

    template<class T>
    ConstTypedObjectReference<T>::ConstTypedObjectReference(const T* obj) : obj(obj)
    {}

    template<class T>
    ConstTypedObjectReference<T>::ConstTypedObjectReference(const T& obj) : obj(&obj)
    {}

    template<class T>
    ConstTypedObjectReference<T>::ConstTypedObjectReference(T* obj) : obj(obj)
    {}

    template<class T>
    ConstTypedObjectReference<T>::ConstTypedObjectReference(T& obj) : obj(&obj)
    {}

    template<class T>
    template<class U, typename std::enable_if<!std::is_same<U, Object>::value, int>::type>
    ConstTypedObjectReference<T>::ConstTypedObjectReference(ConstObjectReference& reference) : obj(static_cast<const T*>(reference.obj))
    {}

    template<class T>
    ConstTypedObjectReference<T>::ConstTypedObjectReference(ConstTypedObjectReference&& arg) : obj(arg.obj)
    {
        arg.obj = nullptr;
    }

    template<class T>
    ConstTypedObjectReference<T>& ConstTypedObjectReference<T>::operator=(ConstTypedObjectReference&& arg)
    {
        obj = arg.obj;
        arg.obj = nullptr;
        return *this;
    }

    template<class T>
    ConstTypedObjectReference<T>& ConstTypedObjectReference<T>::operator=(const T* obj)
    {
        this->obj = obj;
        return *this;
    }

    template<class T>
    ConstTypedObjectReference<T>& ConstTypedObjectReference<T>::operator=(const T& obj)
    {
        this->obj = &obj;
        return *this;
    }

    template<class T>
    bool ConstTypedObjectReference<T>::operator==(const ConstTypedObjectReference& arg) const
    {
        return obj == arg.obj;
    }

    template<class T>
    bool ConstTypedObjectReference<T>::operator!=(const ConstTypedObjectReference& arg) const
    {
        return obj != arg.obj;
    }

    template<class T>
    ConstTypedObjectReference<T>::operator bool() const
    {
        return IsOccupied();
    }

    template<class T>
    ConstTypedObjectReference<T>::operator ConstObjectReference() const
    {
        if (!IsOccupied())
            return ObjectReference();

        ObjectReference ret;
        ret.obj = obj;
        return ret;
    }

    template<class T>
    const T* ConstTypedObjectReference<T>::operator->() const
    {
        return Get();
    }

    template<class T>
    const T& ConstTypedObjectReference<T>::operator*() const
    {
        return *Get();
    }

    template<class T>
    void ConstTypedObjectReference<T>::Reset()
    {
        obj = nullptr;
    }

    template<class T>
    const T* ConstTypedObjectReference<T>::Get() const
    {
        return obj;
    }

    template<class T>
    bool ConstTypedObjectReference<T>::IsOccupied() const
    {
        return Get() != nullptr;
    }

    template<class T>
    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(ConstTypedObjectReference<T>)
    {
        scribe.UnowningPointer(obj);
    }
}

namespace std
{
    template<class T>
    struct is_pointer<::Atmos::TypedObjectReference<T>> : public std::true_type
    {};

    template<class T>
    struct is_pointer<::Atmos::ConstTypedObjectReference<T>> : public std::true_type
    {};

    template<class T>
    struct hash<Atmos::TypedObjectReference<T>>
    {
        typedef Atmos::TypedObjectReference<T> argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<const T*>()(arg.Get()));
            return first;
        }
    };

    template<class T>
    struct hash<Atmos::ConstTypedObjectReference<T>>
    {
        typedef Atmos::ConstTypedObjectReference<T> argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<const T*>()(arg.Get()));
            return first;
        }
    };
}