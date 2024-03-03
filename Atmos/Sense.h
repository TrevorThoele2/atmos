#pragma once

#include "AxisAlignedObject.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class Sense : public AxisAlignedObject
    {
    public:
        StoredProperty<bool> enabled;
    public:
        Sense(ObjectManager& manager);
        Sense(const Sense& arg) = default;
        Sense(const ::Inscription::BinaryTableData<Sense>& data);
        virtual ~Sense() = 0;
    public:
        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<Sense> : ObjectTraitsBase<Sense>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<AxisAlignedObject> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Sense, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Sense, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Sense, BinaryArchive> :
        public ObjectScribe<::Atmos::Sense, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}