
#include "Tile.h"

#include "WorldManager.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\List.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Tile)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(pos);
            scribe.Save(spriteList);
            scribe.Save(flags);
        }
        else // INPUT
        {
            GridPosition loadPos;
            scribe.Load(loadPos);
            scribe.Load(spriteList);
            scribe.Load(flags);

            SetPosition(loadPos);
            spriteList.SetOwnerPosition(&spritePosition);
        }
    }

    void Tile::AttemptPushSprite(Sprite &sprite)
    {
        if(PushesSprites() && handler)
            handler->GetField().renderFragments.Add(sprite);
    }

    void Tile::PullSprite(Sprite &sprite)
    {
        if(handler)
            handler->GetField().renderFragments.Remove(sprite);
    }

    Tile::Tile(const GridPosition &pos) : pos(pos), spritePosition(pos), spriteList(&spritePosition)
    {}

    Tile::Tile(const Tile &arg) : pos(arg.pos), spriteList(arg.spriteList, &spritePosition), handler(arg.handler.wrapped), flags(arg.flags), spritePosition(arg.pos)
    {}

    Tile::Tile(Tile &&arg) : pos(std::move(arg.pos)), spriteList(std::move(arg.spriteList), &spritePosition), handler(arg.handler.wrapped), flags(std::move(arg.flags)), spritePosition(arg.pos)
    {}

    Tile& Tile::operator=(const Tile &arg)
    {
        pos = arg.pos;
        spriteList = arg.spriteList;
        handler = arg.handler;
        flags = arg.flags;
        spritePosition = arg.pos;
        return *this;
    }

    Tile& Tile::operator=(Tile &&arg)
    {
        pos = std::move(arg.pos);
        spriteList = std::move(arg.spriteList);
        handler = std::move(arg.handler);
        flags = std::move(arg.flags);
        spritePosition = arg.pos;
        return *this;
    }

    bool Tile::operator==(const Tile &arg) const
    {
        return pos == arg.pos && spriteList == arg.spriteList && handler == arg.handler && flags == arg.flags && spritePosition == arg.spritePosition;
    }

    bool Tile::operator!=(const Tile &arg) const
    {
        return !(*this == arg);
    }

    Tile::SpriteList::ID Tile::CreateSprite()
    {
        Sprite sprite;
        sprite.SetPosition(spritePosition);

        auto &added = spriteList.Add(std::move(sprite)).second;
        AttemptPushSprite(added->second.Get());
        return added->first;
    }

    Tile::SpriteList::ID Tile::CreateSprite(SpriteList::ID id)
    {
        Sprite sprite;
        sprite.SetPosition(spritePosition);

        auto &added = spriteList.Add(id, std::move(sprite)).second;
        AttemptPushSprite(added->second.Get());
        return added->first;
    }

    Tile::SpriteList::ID Tile::CreateSprite(const Sprite &sprite)
    {
        return CreateSprite(sprite, 0, 0, 0);
    }

    Tile::SpriteList::ID Tile::CreateSprite(const FileName &file, Sprite::Index index, const Color &color, SpriteOffsetT offsetX, SpriteOffsetT offsetY, SpriteOffsetT offsetZ)
    {
        return CreateSprite(Sprite(file, index, color), offsetX, offsetY, offsetZ);
    }

    Tile::SpriteList::ID Tile::CreateSprite(const Sprite &sprite, SpriteOffsetT offsetX, SpriteOffsetT offsetY, SpriteOffsetT offsetZ)
    {
        auto z = sprite.GetZ();
        z -= GetPosition().z;

        Sprite nSprite(sprite);
        nSprite.SetPosition(spritePosition);
        auto &added = spriteList.Add(std::move(nSprite), offsetX, offsetY, offsetZ).second;
        AttemptPushSprite(added->second.Get());
        return added->first;
    }

    Tile::SpriteList::ID Tile::CreateSprite(const SpriteList::HandleT &handle)
    {
        auto &added = spriteList.Add(handle).second;
        AttemptPushSprite(added->second.Get());
        return added->first;
    }

    Tile::SpriteList::ID Tile::CreateSprite(SpriteList::ID id, const SpriteList::HandleT &handle)
    {
        auto &added = spriteList.Add(id, handle).second;
        AttemptPushSprite(added->second.Get());
        return added->first;
    }

    Tile::SpriteList::HandleT* Tile::FindSprite(SpriteList::ID id)
    {
        auto found = spriteList.Find(id);
        if (found == spriteList.end())
            return nullptr;

        return &found->second;
    }

    const Tile::SpriteList::HandleT* Tile::FindSprite(SpriteList::ID id) const
    {
        auto found = spriteList.Find(id);
        if (found == spriteList.end())
            return nullptr;

        return &found->second;
    }

    void Tile::DestroySprite(SpriteList::HandleT &destroy)
    {
        spriteList.Remove(destroy);
    }

    void Tile::DestroySprite(SpriteList::ID destroy)
    {
        spriteList.Remove(destroy);
    }

    void Tile::DestroySprite(SpriteList::iterator destroy)
    {
        spriteList.Remove(destroy);
    }

    Tile::SpriteList::ID Tile::GetNextAutoSpriteID() const
    {
        return spriteList.GetNextAutoID();
    }

    void Tile::SetPosition(const GridPosition &set)
    {
        pos = set;
        spritePosition = pos;
    }

    const GridPosition& Tile::GetPosition() const
    {
        return pos;
    }

    void Tile::GetSurrounding(std::unordered_set<const Tile*> &set) const
    {
        std::unordered_set<GridPosition> positions;
        GetSurroundingPlane(positions, GetPosition());

        for (auto loop = positions.begin(); loop != positions.end(); loop++)
        {
            auto found = handler->Find(*loop);
            if (found)
                set.emplace(found);
        }
    }

    void Tile::FindSquare(std::unordered_set<const Tile*> &set, unsigned short range) const
    {
        std::unordered_set<GridPosition> positions;
        FindPosSquarePlane(positions, GetPosition(), range);

        for (auto loop = positions.begin(); loop != positions.end(); loop++)
        {
            auto found = handler->Find(*loop);
            if (found != nullptr)
                set.emplace(found);
        }
    }

    void Tile::SetSolid(bool setTo)
    {
        flags.Set(Flag::SOLID, setTo);
    }

    bool Tile::IsSolid() const
    {
        return flags.Get(Flag::SOLID);
    }

    bool Tile::CanMoveInto() const
    {
        return !IsSolid();
    }

    Tile::SpriteList& Tile::GetSpriteList()
    {
        return spriteList;
    }

    const Tile::SpriteList& Tile::GetSpriteList() const
    {
        return spriteList;
    }
    
    void Tile::PushSpritesToHandler()
    {
        flags.Set(Flag::PUSH_SPRITES, true);
        for (auto &sprite : spriteList)
            AttemptPushSprite(*sprite.second);
    }

    void Tile::PullSpritesFromHandler()
    {
        flags.Set(Flag::PUSH_SPRITES, false);
        for (auto &sprite : spriteList)
            PullSprite(*sprite.second);
    }

    bool Tile::PushesSprites() const
    {
        return flags.Get(Flag::PUSH_SPRITES);
    }
}