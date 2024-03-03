#pragma once

#include "Object.h"

#include "Sprite.h"
#include "PositionalOffsetAdapter.h"
#include "GridPosition.h"

#include "ReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos::Grid
{
    class Tile : public Object
    {
    public:
        using Position = Position;
        using PositionProperty = ReadonlyProperty<Position>;
        PositionProperty position = PositionProperty(
            [this]() -> Position& { return _position; });
    public:
        using OffsetSprite = PositionalOffsetAdapter<Render::Sprite>;
        using SpriteList = std::vector<OffsetSprite>;
        using SpriteListProperty = ReadonlyProperty<SpriteList&>;
        SpriteListProperty sprites = SpriteListProperty(
            [this]() -> SpriteList& { return _spriteList; });
    public:
        using SolidProperty = StoredProperty<bool>;
        SolidProperty solid;
    public:
        Tile(ObjectManager& manager, const Position& position);
        Tile(const Tile& arg) = default;
        Tile(const ::Inscription::BinaryTableData<Tile>& data);

        ObjectTypeDescription TypeDescription() const override;
    private:
        Position _position;
        SpriteList _spriteList;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Grid::Tile> : ObjectTraitsBase<Grid::Tile>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Grid::Tile, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Grid::Tile, BinaryArchive>
    {
        ObjectT::Position position;
        ObjectT::SpriteList spriteList;
        bool solid;
    };

    template<>
    class Scribe<::Atmos::Grid::Tile, BinaryArchive> : public ObjectScribe<::Atmos::Grid::Tile, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}