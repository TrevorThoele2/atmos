
#pragma once

#include "Object.h"

#include "Sprite.h"
#include "ObjectOffset.h"
#include "GridPosition.h"

#include "ReadonlyProperty.h"
#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nTile : public Object
    {
    public:
        typedef StoredReadonlyProperty<GridPosition> GridPositionProperty;
        GridPositionProperty position;
    public:
        typedef PositionalOffsetAdapter<nSprite> OffsetSprite;
        typedef std::vector<OffsetSprite> SpriteList;
        typedef ReadonlyProperty<SpriteList&> SpriteListProperty;
        SpriteListProperty sprites;
    public:
        typedef StoredProperty<bool> SolidProperty;
        SolidProperty solid;
    public:
        nTile(const GridPosition& position);
        nTile(const nTile& arg) = default;
        nTile(const ::Inscription::Table<nTile>& table);

        ObjectTypeDescription TypeDescription() const override;
    private:
        SpriteList _spriteList;
    private:
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<nTile> : ObjectTraitsBase<nTile>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nTile)
    {
    public:
        static void AddMembers(TableT& table);
    };
}