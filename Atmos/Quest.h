#pragma once

#include <vector>

#include "RegistryObject.h"

#include "QuestPart.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nQuest : public RegistryObject
    {
    public:
        typedef std::vector<QuestPart> PartList;
    public:
        PartList parts;
        PartList::iterator curPos;
    public:
        nQuest(const Name& name);
        nQuest(const nQuest& arg) = default;
        nQuest(const ::Inscription::Table<nQuest>& table);

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nQuest> : ObjectTraitsBase<nQuest>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RegistryObject> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nQuest)
    {
    public:
        static void AddMembers(TableT& table);
    };
}