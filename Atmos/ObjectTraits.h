#pragma once

#include "ObjectTypeDescription.h"
#include "ObjectTypeList.h"
#include "ObjectTypeName.h"

#include <Chroma/Iterate.h>

namespace Atmos
{
    template<class T>
    struct ObjectTraits
    {
        static const ObjectTypeName typeName;
    };

    template<class T>
    const ObjectTypeName ObjectTraits<T>::typeName = "";

    template<class T>
    ObjectTypeName TypeNameFor()
    {
        return ObjectTraits<T>::typeName;
    }

    template<class T>
    ObjectTypeDescription TypeDescriptionFor()
    {
        return ObjectTraits<T>::TypeDescription();
    }

    template<class T>
    struct ObjectTraitsBase
    {
        static ObjectTypeDescription TypeDescription();

        template <class U, class = int>
        struct HasBaseList : std::false_type
        {};

        template <class U>
        struct HasBaseList<U, decltype((void)ObjectTraits<U>::bases, 0)> : std::true_type
        {};

        template<template<::Chroma::VariadicTemplateSize> class Iterator, class... BaseTypes, class... Args>
        static void IterateOverBases(const ObjectTypeList<BaseTypes...>& baseList, Args && ... args)
        {
            typedef ObjectTypeList<BaseTypes...> BaseListT;

            ::Chroma::IterateRange<::Chroma::VariadicTemplateSize, Iterator, BaseListT::count - 1, 0>(baseList, std::forward<Args>(args)...);
        }
    private:
        template<::Chroma::VariadicTemplateSize i>
        struct BaseIterator
        {
            template<class... BaseTypes>
            static void Do(const ObjectTypeList<BaseTypes...>& baseList, ObjectTypeDescription::BaseTypeList& fillList)
            {
                typedef ObjectTypeList<BaseTypes...> BaseListT;
                typedef ObjectTraits<typename BaseListT::template Parameter<i>::Type> BaseTraitsT;

                fillList.push_back(BaseTraitsT::typeName);
            }
        };

        template<class U, typename std::enable_if<HasBaseList<U>::value, int>::type = 0>
        static void AttemptFillBases(ObjectTypeDescription::BaseTypeList& fillList)
        {
            IterateOverBases<BaseIterator>(ObjectTraits<T>::bases, fillList);
        }

        template<class U, typename std::enable_if<!HasBaseList<U>::value, int>::type = 0>
        static void AttemptFillBases(ObjectTypeDescription::BaseTypeList& list)
        {}
    };

    template<class T>
    ObjectTypeDescription ObjectTraitsBase<T>::TypeDescription()
    {
        ObjectTypeDescription::BaseTypeList baseList;
        AttemptFillBases<T>(baseList);
        return ObjectTypeDescription(ObjectTraits<T>::typeName, baseList);
    }

    template<class T>
    struct ObjectTraits;
}