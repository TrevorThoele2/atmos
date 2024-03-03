
#pragma once

#include "Object.h"

#include "Sprite.h"
#include "PositionalOffsetAdapter.h"
#include "GridPosition.h"

#include "ReadonlyProperty.h"
#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class Tile : public Object
    {
    public:
        typedef StoredReadonlyProperty<GridPosition> GridPositionProperty;
        GridPositionProperty position;
    public:
        typedef PositionalOffsetAdapter<Sprite> OffsetSprite;
        typedef std::vector<OffsetSprite> SpriteList;
        typedef ReadonlyProperty<SpriteList&> SpriteListProperty;
        SpriteListProperty sprites;
    public:
        typedef StoredProperty<bool> SolidProperty;
        SolidProperty solid;
    public:
        Tile(ObjectManager& manager, const GridPosition& position);
        Tile(const Tile& arg) = default;
        Tile(const ::Inscription::Table<Tile>& table);

        ObjectTypeDescription TypeDescription() const override;
    private:
        SpriteList _spriteList;
    private:
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<Tile> : ObjectTraitsBase<Tile>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Tile)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}