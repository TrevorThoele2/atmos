#pragma once

#include <functional>

#include "ArithmeticPropertySlotInterface.h"
#include "PointerPropertySlotInterface.h"
#include "EqualityPropertySlotInterface.h"
#include "ListPropertySlotInterface.h"

#include "DifferentiatableInherit.h"
#include "HasEqualityComparison.h"
#include "IsList.h"

#include "Event.h"

namespace Atmos
{
    template<class T>
    using AttemptArithmeticSlot =
        typename std::conditional<
            std::is_arithmetic<typename std::remove_reference<T>::type>::value,
            ArithmeticPropertySlotInterface<T>,
            DifferentiatableInherit<0>>::type;
    template<class T>
    using AttemptPointerSlot =
        typename std::conditional<
            std::is_pointer<typename std::remove_reference<T>::type>::value,
            PointerPropertySlotInterface<T>,
            DifferentiatableInherit<1>>::type;
    template<class T>
    using AttemptEqualitySlot =
        typename std::conditional<
            HasEqualityComparison<typename std::remove_reference<T>::type>::value,
            EqualityPropertySlotInterface<T>,
            DifferentiatableInherit<2>>::type;
    template<class T>
    using AttemptListSlot =
        typename std::conditional<
            IsList<typename std::remove_reference<T>::type>::value,
            ListPropertySlotInterface<T>,
            DifferentiatableInherit<3>>::type;

    template<class T>
    class Property :
        public virtual PropertyCore<T>,
        public virtual AttemptArithmeticSlot<T>,
        public virtual AttemptPointerSlot<T>,
        public virtual AttemptEqualitySlot<T>,
        public virtual AttemptListSlot<T>
    {
    public:
        typedef T Value;
    public:
        Event<T> onBeforeValueChanged;
        Event<T> onValueChanged;
    private:
        typedef std::function<T()> Getter;
        typedef std::function<void(T)> Setter;
    public:
        Property(Getter getter, Setter setter);
        Property(const Property& arg) = default;
        Property(Property&& arg);

        Property& operator=(const Property& arg);
        Property& operator=(T newValue);

        operator T() const;

        Property& Set(T newValue);
        T Get() const;
    protected:
        T GetInternal() const override;
        void SetInternal(T set) override;
    private:
        Getter getter;
        Setter setter;
    };

    template<class T>
    Property<T>::Property(Getter getter, Setter setter) : getter(getter), setter(setter)
    {}

    template<class T>
    Property<T>::Property(Property&& arg) : getter(std::move(arg.getter)), setter(std::move(arg.setter))
    {}

    template<class T>
    Property<T>& Property<T>::operator=(const Property& arg)
    {
        SetInternal(arg.GetInternal());
        return *this;
    }

    template<class T>
    Property<T>& Property<T>::operator=(T newValue)
    {
        SetInternal(newValue);
        return *this;
    }

    template<class T>
    Property<T>::operator T() const
    {
        return GetInternal();
    }

    template<class T>
    Property<T>& Property<T>::Set(T newValue)
    {
        SetInternal(newValue);
        return *this;
    }

    template<class T>
    T Property<T>::Get() const
    {
        return GetInternal();
    }

    template<class T>
    T Property<T>::GetInternal() const
    {
        return getter();
    }

    template<class T>
    void Property<T>::SetInternal(T set)
    {
        onBeforeValueChanged(GetInternal());
        setter(set);
        onValueChanged(GetInternal());
    }
}

namespace std
{
    template<class T>
    struct is_arithmetic<::Atmos::Property<T>> : public std::integral_constant<bool, std::is_arithmetic<T>::value>
    {};

    template<class T>
    struct is_pointer<::Atmos::Property<T>> : public std::integral_constant<bool, std::is_pointer<T>::value>
    {};
}

namespace Atmos
{
    template<class T>
    struct IsList<::Atmos::Property<T>> : public std::integral_constant<bool, IsList<T>::value>
    {};
}