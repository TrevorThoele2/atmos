#pragma once

#include "nEntityComponent.h"

#include "NameValuePair.h"
#include "GridPosition.h"
#include "Direction.h"
#include "Tag.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Ent
    {
        class nGeneralComponent : public nEntityComponent
        {
        public:
            typedef NameValuePair StorageObject;
        public:
            // Name
            Name name;
            Name niceName;

            // Position
            typedef GridPosition PositionT;
            PositionT position;

            // Direction
            typedef Direction DirectionT;
            DirectionT direction;

            // Solid
            bool solid;

            // Persistent storage
            std::unordered_map<Name, StorageObject> storage;

            // Tags
            std::unordered_set<Tag> tags;
        public:
            nGeneralComponent(EntityReference reference);
            nGeneralComponent(const nGeneralComponent& arg) = default;
            nGeneralComponent(const ::Inscription::Table<nGeneralComponent>& table);

            void SetPosition(const PositionT &set);
            GridPosition GetPositionInFront() const;

            void SetSolid(bool set);
            bool IsSolid() const;

            StorageObject* AddStorage(const StorageObject &add);
            StorageObject* AddStorage(StorageObject &&add);
            void RemoveStorage(const Name &remove);
            void RemoveStorage(const StorageObject &remove);

            StorageObject* FindStorage(const Name &find);
            StorageObject* FindStorage(const StorageObject &find);
            const StorageObject* FindStorage(const Name &find) const;
            const StorageObject* FindStorage(const StorageObject &find) const;
            bool HasStorage(const Name &check) const;
            bool HasStorage(const StorageObject &check) const;

            void TagAs(const Tag &add);
            void RemoveTag(const Tag &remove);
            bool IsTaggedAs(const Tag &check) const;

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Ent::nGeneralComponent> : ObjectTraitsBase<Ent::nGeneralComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nGeneralComponent)
    {
    public:
        static void AddMembers(TableT& table);

        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}