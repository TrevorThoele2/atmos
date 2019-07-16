#pragma once

#include "ObjectStaticAssert.h"

#include "Serialization.h"

namespace Inscription
{
    template<class T, class Archive>
    class ObjectSystemScribe : public CompositeScribe<T, BinaryArchive>
    {
    private:
        using BaseT = TableScribe<T, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        using ClassNameResolver = typename BaseT::ClassNameResolver;
        template<class T>
        using ComposingScribe = typename BaseT::template ComposingScribe<T>;
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(T);
    };
}