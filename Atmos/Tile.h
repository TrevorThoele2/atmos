
#pragma once

#include "GridPosition.h"
#include "Sprite.h"
#include "OffsetManagedList.h"

#include "Flags.h"
#include "ObjectHandle.h"
#include "AccessAdapter.h"

#include "Serialization.h"

namespace Atmos
{
    class TileHandler;
    class Tile
    {
    public:
        typedef OffsetManagedList<Sprite> SpriteList;
        typedef SpriteList::OffsetValueT SpriteOffsetT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend TileHandler;

        enum class Flag : unsigned char
        {
            SOLID = 1 << 0,
            PUSH_SPRITES = 1 << 1
        };
    private:
        GridPosition pos;

        Position3D spritePosition;
        SpriteList spriteList;

        Flags<Flag> flags;

        void AttemptPushSprite(Sprite &sprite);
        void PullSprite(Sprite &sprite);
    public:
        ObjectHandle<TileHandler, Tile> handler;

        Tile(const GridPosition &pos);
        Tile(const Tile &arg);
        Tile(Tile &&arg);
        Tile& operator=(const Tile &arg);
        Tile& operator=(Tile &&arg);
        bool operator==(const Tile &arg) const;
        bool operator!=(const Tile &arg) const;

        SpriteList::ID CreateSprite();
        SpriteList::ID CreateSprite(SpriteList::ID id);
        SpriteList::ID CreateSprite(const Sprite &sprite);
        SpriteList::ID CreateSprite(const FileName &file, Sprite::Index index, const Color &color, SpriteOffsetT offsetX, SpriteOffsetT offsetY, SpriteOffsetT offsetZ);
        SpriteList::ID CreateSprite(const Sprite &sprite, SpriteOffsetT offsetX, SpriteOffsetT offsetY, SpriteOffsetT offsetZ);
        SpriteList::ID CreateSprite(const SpriteList::HandleT &handle);
        SpriteList::ID CreateSprite(SpriteList::ID id, const SpriteList::HandleT &handle);
        SpriteList::HandleT* FindSprite(SpriteList::ID id);
        const SpriteList::HandleT* FindSprite(SpriteList::ID id) const;
        void DestroySprite(SpriteList::HandleT &destroy);
        void DestroySprite(SpriteList::ID destroy);
        void DestroySprite(SpriteList::iterator destroy);
        SpriteList::ID GetNextAutoSpriteID() const;

        void SetPosition(const GridPosition &set);
        const GridPosition& GetPosition() const;
        void GetSurrounding(std::unordered_set<const Tile*> &set) const;
        void FindSquare(std::unordered_set<const Tile*> &set, unsigned short range) const;

        void SetSolid(bool setTo);
        bool IsSolid() const;
        bool CanMoveInto() const;

        SpriteList& GetSpriteList();
        const SpriteList& GetSpriteList() const;
        void PushSpritesToHandler();
        void PullSpritesFromHandler();
        bool PushesSprites() const;
    };
}