#pragma once

#include "RegistryObject.h"

#include "AttackRange.h"
#include "Acumen.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nAbility : public RegistryObject
    {
    public:
        AttackRange range;
        Acumen cost;
    public:
        nAbility(ObjectManager& manager, const Name& name);
        nAbility(const nAbility& arg) = default;
        nAbility(const ::Inscription::Table<nAbility>& table);

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nAbility> : ObjectTraitsBase<nAbility>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RegistryObject> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nAbility)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}