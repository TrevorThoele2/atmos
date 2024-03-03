#pragma once

#include "ObjectStaticAssert.h"

#include "Serialization.h"

namespace Inscription
{
    template<class T, class Archive>
    class ObjectSystemScribe : public CompositeScribe<T, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<T, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        using ClassNameResolver = typename BaseT::ClassNameResolver;
    protected:
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive);

        using BaseT::DoBasicConstruction;
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(T);
    };

    template<class T, class Archive>
    void ObjectSystemScribe<T, Archive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}