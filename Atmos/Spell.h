#pragma once

#include <vector>

#include "RegistryObject.h"

#include "BattlePatternHolder.h"
#include "AttackRange.h"
#include "ResourceAttribute.h"
#include "CharacterClass.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nSpell : public RegistryObject
    {
    public:
        BattlePatternHolder attackPattern;
        BattlePatternHolder::Piece *pieceOverworld;
        AttackRange range;
        Attribute resourceCost;
        typedef TypedObjectReference<nCharacterClass> CharacterClassReference;
        std::vector<CharacterClassReference> allowedClasses;
    public:
        nSpell(const Name& name);
        nSpell(const nSpell& arg) = default;
        nSpell(const ::Inscription::Table<nSpell>& table);

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nSpell> : ObjectTraitsBase<nSpell>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RegistryObject> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nSpell)
    {
    public:
        static void AddMembers(TableT& table);
    };
}