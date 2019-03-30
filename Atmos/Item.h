#pragma once

#include "RegistryObject.h"

#include "ScriptInstance.h"
#include "Sprite.h"

#include "Flags.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nItem : public RegistryObject
    {
    public:
        typedef TypedObjectReference<ScriptInstance> ScriptInstanceReference;
        ScriptInstanceReference consume;
        ScriptInstanceReference equip;
        ScriptInstanceReference attack;
    public:
        typedef TypedObjectReference<nSprite> SpriteReference;
        SpriteReference portrait;
        String description;
    public:
        typedef int Price;
        Price buyPrice;
        Price sellPrice;
    public:
        enum class Flag
        {
            KEY = 1 << 0,
            TEMPORARY = 1 << 1
        };
        Flags<Flag> flags;
    public:
        nItem(const Name& name);
        nItem(const nItem& arg) = default;
        nItem(const ::Inscription::Table<nItem>& table);

        bool IsConsumable() const;
        bool IsEquippable() const;
        bool CanAttackWith() const;

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nItem> : ObjectTraitsBase<nItem>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RegistryObject> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nItem)
    {
    public:
        static void AddMembers(TableT& table);
    };
}