#pragma once

#include <tuple>

#include "ObjectStaticAssert.h"

#include "ObjectTraits.h"
#include "ProcessedObjectTraits.h"
#include "ObjectTypeList.h"
#include "ObjectID.h"

#include "Serialization.h"

#include <Chroma/Iterate.h>

namespace Inscription
{
    template<class T, class Archive>
    class ObjectTableDataBase : public TableDataBase<T, Archive>
    {
    private:
        using BaseT = TableDataBase<T, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        template<class T>
        using Base = typename BaseT::template Base<T>;
    private:
        using Traits = ::Atmos::ProcessedObjectTraits<ObjectT>;
    private:
        template<class BaseT>
        struct BaseTransformer
        {
            using Type = BaseTableDataEntry<BaseT, ObjectT, ArchiveT>;
        };
    public:
        using Bases = typename Traits::Bases::template Transform<BaseTransformer>::Type::TupleT;
        Bases bases = {};
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };

    template<class T, class Archive>
    class ObjectTableBase : public ::Inscription::TableBase<T, Archive>
    {
    private:
        using BaseT = TableBase<T, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        using DataT = typename BaseT::DataT;
        using BaseT::data;
    public:
        ObjectTableBase();
    protected:
        using DataEntry = typename BaseT::DataEntry;
    private:
        using Bases = typename DataT::Bases;
        constexpr static bool baseCount = std::tuple_size<Bases>::value;

        template<size_t i>
        struct BaseIterator
        {
            static void Do(Bases& baseList, ObjectTableBase& table)
            {
                table.AddDataEntry(DataEntry::Base(std::get<i>(baseList)));
            }
        };

        void AddBaseEntries()
        {
            ::Chroma::IterateRange<size_t, BaseIterator, baseCount - 1>(data.bases, *this);
        }
    private:
        STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(T);
    };

    template<class T, class Archive>
    ObjectTableBase<T, Archive>::ObjectTableBase()
    {
        AddBaseEntries();
    }

    template<class T, class Archive>
    class ObjectScribe : public TableScribe<T, Archive>
    {
    private:
        using BaseT = TableScribe<T, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        using ClassNameResolver = typename BaseT::ClassNameResolver;
    public:
        using DataBase = ObjectTableDataBase<ObjectT, ArchiveT>;
        using TableBase = ObjectTableBase<ObjectT, ArchiveT>;
    public:
        static const ClassNameResolver classNameResolver;
    };

    template<class T, class Archive>
    typename const ObjectScribe<T, Archive>::ClassNameResolver ObjectScribe<T, Archive>::classNameResolver([](ArchiveT& archive)
    {
        return ::Atmos::TypeNameFor<T>();
    });
}