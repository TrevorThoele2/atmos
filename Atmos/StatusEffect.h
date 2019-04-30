#pragma once

#include "RegistryObject.h"
#include "ObjectReference.h"

#include "CombatComponent.h"
#include "ScriptInstance.h"

#include "FixedPoint.h"
#include "Name.h"
#include "GlobalContext.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nStatusEffect : public RegistryObject
    {
    public:
        typedef unsigned char ApplicationCount;
        typedef FixedPoint64 Accuracy;
    public:
        typedef TypedObjectReference<Entity::CombatComponent> CombatComponentReference;
    public:
        typedef TypedObjectReference<ScriptInstance> ScriptReference;
        ScriptReference script;
        ApplicationCount applicationCount;
        Name proficiencyName;
        Accuracy accuracy;
        bool positive;
    public:
        nStatusEffect(ObjectManager& manager, const Name& name);
        nStatusEffect(const nStatusEffect& arg) = default;
        nStatusEffect(const ::Inscription::Table<nStatusEffect>& table);

        void Apply(CombatComponentReference applyTo) const;

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nStatusEffect> : ObjectTraitsBase<nStatusEffect>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RegistryObject> bases = {};
    };

    template<>
    class GlobalContext<nStatusEffect> : public GlobalContextBase<nStatusEffect, GlobalContext<nStatusEffect>>
    {
    private:
        typedef GlobalContextBase<nStatusEffect, GlobalContext<nStatusEffect>> BaseT;
        friend GlobalContextBase<nStatusEffect, GlobalContext<nStatusEffect>>;
    private:
        static void SerializeImpl(::Inscription::Scribe &scribe);
    public:
        static Name accuracyStat;
        static Name resistanceStat;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nStatusEffect)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}