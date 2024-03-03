#pragma once

#include "ObjectTypeDescription.h"
#include "ObjectTypeList.h"
#include "ObjectTypeName.h"

#include "ProcessedObjectTraits.h"

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
    private:
        using Processed = ProcessedObjectTraits<T>;

        template<size_t i>
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

        static void AttemptFillBases(ObjectTypeDescription::BaseTypeList& fillList, std::true_type)
        {
            Processed::template IterateOverBases<BaseIterator>(Processed::bases, fillList);
        }

        static void AttemptFillBases(ObjectTypeDescription::BaseTypeList& list, std::false_type)
        {}
    };

    template<class T>
    ObjectTypeDescription ObjectTraitsBase<T>::TypeDescription()
    {
        ObjectTypeDescription::BaseTypeList baseList;
        AttemptFillBases(baseList, Processed::HasCustomBaseList{});
        return ObjectTypeDescription(ObjectTraits<T>::typeName, baseList);
    }

    template<class T>
    struct ObjectTraits;
}