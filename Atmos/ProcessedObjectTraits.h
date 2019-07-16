#pragma once

#include <utility>

#include "ObjectStaticAssert.h"

#include <Chroma/Iterate.h>

namespace Atmos
{
    template<class T>
    struct ObjectTraits;

    template<class T>
    struct ProcessedObjectTraits
    {
    public:
        using UnderlyingTraits = ObjectTraits<T>;

        template<template<size_t> class Iterator, class... BaseTypes, class... Args>
        static void IterateOverBases(const ObjectTypeList<BaseTypes...>& baseList, Args&& ... args)
        {
            using BaseListT = ObjectTypeList<BaseTypes...>;

            ::Chroma::IterateRange<size_t, Iterator, BaseListT::count - 1>(baseList, std::forward<Args>(args)...);
        }
    private:
        template<class U, class = int>
        struct DiscoverHasCustomBaseList : std::false_type
        {};

        template<class U>
        struct DiscoverHasCustomBaseList<U, decltype((void)ObjectTraits<U>::bases, 0)> : std::true_type
        {};

        template<class U, bool>
        struct BaseList;

        template<class U>
        struct BaseList<U, true>
        {
            using Type = decltype(UnderlyingTraits::bases);
        };

        template<class U>
        struct BaseList<U, false>
        {
            using Type = ObjectTypeList<Object>;
        };
    public:
        using HasCustomBaseList = DiscoverHasCustomBaseList<T>;
        using Bases = typename BaseList<T, HasCustomBaseList::value>::Type;
    private:
        template<class U>
        struct DiscoverBaseList
        {
            constexpr static Bases bases = {};
        };
    public:
        constexpr static Bases bases = DiscoverBaseList<T>::bases;
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };
}