#pragma once

#include <unordered_set>

#include "EntityComponent.h"

#include "NameValuePair.h"
#include "GridPosition.h"
#include "Direction.h"
#include "Tag.h"

#include "ObjectScribe.h"

namespace Atmos::Entity
{
    class GeneralComponent : public Component
    {
    public:
        typedef NameValuePair StorageObject;
    public:
        // Name
        Name name;
        Name niceName;

        // Position
        typedef GridPosition Position;
        Position position;

        // Direction
        typedef Direction Direction;
        Direction direction;

        // Solid
        bool solid;

        // Persistent storage
        std::unordered_map<Name, StorageObject> storage;

        // Tags
        std::unordered_set<Tag> tags;
    public:
        GeneralComponent(ObjectManager& manager, EntityReference reference);
        GeneralComponent(const GeneralComponent& arg) = default;
        GeneralComponent(const ::Inscription::BinaryTableData<GeneralComponent>& data);

        void SetPosition(const Position &set);
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

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::GeneralComponent> : ObjectTraitsBase<Entity::GeneralComponent>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Entity::Component> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::GeneralComponent, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::GeneralComponent, BinaryArchive>
    {
        ::Atmos::Name name;
        ::Atmos::Name niceName;
        ObjectT::Position position;
        ObjectT::Direction direction;
        bool solid;
        std::unordered_set<::Atmos::Tag> tags;
    };

    template<>
    class Scribe<::Atmos::Entity::GeneralComponent, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::GeneralComponent, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };
}