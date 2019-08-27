#pragma once

#include "ObjectFixture.h"

#include <Inscription/BinaryArchive.h>

#include <Atmos/AxisAlignedObject.h>

using namespace Atmos;

class PositionalOffsetAdapterTestsFixture : public ObjectFixture
{
public:
    PositionalOffsetAdapterTestsFixture();

    class DerivedObject : public AxisAlignedObject
    {
    public:
        int number;
    public:
        DerivedObject(ObjectManager& manager);

        DerivedObject(const ::Inscription::BinaryTableData<DerivedObject>& data);

        ObjectTypeDescription TypeDescription() const override;
    };
};

namespace Atmos
{
    template<>
    struct ObjectTraits<PositionalOffsetAdapterTestsFixture::DerivedObject> :
        ObjectTraitsBase<PositionalOffsetAdapterTestsFixture::DerivedObject>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<AxisAlignedObject> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<PositionalOffsetAdapterTestsFixture::DerivedObject, BinaryArchive> :
        public ObjectTableDataBase<PositionalOffsetAdapterTestsFixture::DerivedObject, BinaryArchive>
    {
        int number;
    };

    template<>
    class Scribe<PositionalOffsetAdapterTestsFixture::DerivedObject, BinaryArchive> :
        public ObjectScribe<PositionalOffsetAdapterTestsFixture::DerivedObject, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}