#pragma once

#include <functional>

#include "ReadonlyPointerPropertySlotInterface.h"
#include "EqualityPropertySlotInterface.h"
#include "ListPropertySlotInterface.h"

#include "DifferentiatableInherit.h"
#include "HasEqualityComparison.h"
#include "IsList.h"

#include "Event.h"

namespace Atmos
{
    template<class T>
    using AttemptReadonlyPointerSlot =
        typename std::conditional<
            std::is_pointer<typename std::remove_reference<T>::type>::value,
            ReadonlyPointerPropertySlotInterface<T>,
            DifferentiatableInherit<0>>::type;
    template<class T>
    using AttemptReadonlyEqualitySlot =
        typename std::conditional<HasEqualityComparison<typename std::remove_reference<T>::type>::value,
            EqualityPropertySlotInterface<T>,
            DifferentiatableInherit<1>>::type;
    template<class T>
    using AttemptReadonlyListSlot =
        typename std::conditional<
            IsList<typename std::remove_reference<T>::type>::value,
            ListPropertySlotInterface<T>,
            DifferentiatableInherit<2>>::type;

    template<class T>
    class ReadonlyProperty :
        public virtual PropertyCore<T>,
        public virtual AttemptReadonlyPointerSlot<T>,
        public virtual AttemptReadonlyEqualitySlot<T>,
        public virtual AttemptReadonlyListSlot<T>
    {
    public:
        typedef T Value;
    private:
        typedef std::function<T()> Getter;
    public:
        ReadonlyProperty(Getter getter);
        ReadonlyProperty(const ReadonlyProperty& arg) = default;
        ReadonlyProperty(ReadonlyProperty&& arg);

        operator T() const;
        T Get() const;
    protected:
        T GetInternal() const override;
        void SetInternal(T set) override;
    private:
        Getter getter;
    };

    template<class T>
    ReadonlyProperty<T>::ReadonlyProperty(Getter getter) : getter(getter)
    {}

    template<class T>
    ReadonlyProperty<T>::ReadonlyProperty(ReadonlyProperty&& arg) : getter(std::move(arg.getter))
    {}

    template<class T>
    ReadonlyProperty<T>::operator T() const
    {
        return GetInternal();
    }

    template<class T>
    T ReadonlyProperty<T>::Get() const
    {
        return GetInternal();
    }

    template<class T>
    T ReadonlyProperty<T>::GetInternal() const
    {
        return getter();
    }

    template<class T>
    void ReadonlyProperty<T>::SetInternal(T set)
    {}
}