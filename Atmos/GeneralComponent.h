#pragma once

#include <unordered_set>

#include <Arca/Shard.h>
#include <Arca/NullScribe.h>

#include "NameValuePair.h"
#include "GridPosition.h"
#include "Direction.h"
#include "Tag.h"

namespace Atmos::Entity
{
    class GeneralComponent
    {
    public:
        using StorageObject = NameValuePair;
    public:
        Name name;
        Name displayName;

        using Position = Grid::Position;
        Position position;

        Direction direction;

        bool solid = false;

        std::unordered_map<Name, StorageObject> storage;

        std::unordered_set<Tag> tags;
    public:
        GeneralComponent() = default;

        void SetPosition(const Position& set);
        [[nodiscard]] Grid::Position PositionInFront() const;

        void SetSolid(bool set = true);
        [[nodiscard]] bool IsSolid() const;

        StorageObject* AddStorage(const StorageObject& add);
        StorageObject* AddStorage(StorageObject&& add);
        void RemoveStorage(const Name& remove);
        void RemoveStorage(const StorageObject& remove);

        [[nodiscard]] StorageObject* FindStorage(const Name& find);
        [[nodiscard]] const StorageObject* FindStorage(const Name& find) const;
        [[nodiscard]] bool HasStorage(const Name& check) const;
        [[nodiscard]] bool HasStorage(const StorageObject& check) const;

        void TagAs(const Tag& add);
        void RemoveTag(const Tag& remove);
        [[nodiscard]] bool IsTaggedAs(const Tag& check) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Entity::GeneralComponent>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Atmos::Entity::GeneralComponent";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::GeneralComponent, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::GeneralComponent, BinaryArchive>
    {};
}