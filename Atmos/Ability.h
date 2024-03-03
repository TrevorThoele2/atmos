#pragma once

#include "RegistryObject.h"

#include "BattlePatternHolder.h"
#include "AttackRange.h"
#include "Acumen.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nAbility : public RegistryObject
    {
    public:
        BattlePatternHolder pattern;
        AttackRange range;
        Acumen cost;
    public:
        nAbility(const Name& name);
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
        static void AddMembers(TableT& table);
    };
}