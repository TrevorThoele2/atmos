#pragma once

#include <vector>

#include "RegistryObject.h"

#include "AttackRange.h"
#include "ResourceAttribute.h"
#include "CharacterClass.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nSpell : public RegistryObject
    {
    public:
        AttackRange range;
        Attribute resourceCost;
        typedef TypedObjectReference<nCharacterClass> CharacterClassReference;
        std::vector<CharacterClassReference> allowedClasses;
    public:
        nSpell(ObjectManager& manager, const Name& name);
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
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}