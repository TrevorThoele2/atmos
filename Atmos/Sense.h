#pragma once

#include "AxisAlignedObject.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class Sense : public AxisAlignedObject
    {
    public:
        StoredProperty<bool> enabled;
    public:
        Sense(ObjectManager& manager);
        Sense(const Sense& arg) = default;
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(Sense);
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
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Sense)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}