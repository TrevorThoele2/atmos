
#include "TileHandler.h"

#include "Camera.h"
#include "WorldManager.h"
#include "GridSize.h"

#include <Inscription/Scribe.h>
#include <Inscription/Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TileHandler)
    {
        (scribe.IsOutput()) ? Save(scribe) : Load(scribe);
    }

    void TileHandler::Save(::Inscription::Scribe &scribe)
    {
        ::Inscription::ContainerSize size(container.size());
        scribe.Save(size);

        for (auto &loop : container)
            scribe.Save(loop.second);
    }

    void TileHandler::Load(::Inscription::Scribe &scribe)
    {
        const GridPosition tilePosition;

        ::Inscription::ContainerSize size;
        scribe.Load(size);

        container.clear();
        while (size-- > 0)
        {
            Tile tile(tilePosition);
            scribe.Load(tile);
            Add(std::move(tile));
        }
    }

    void TileHandler::MoveContainer(Map &&arg)
    {
        container = std::move(arg);
        for (auto &loop : container)
            loop.second.handler = this;
    }

    TileHandler::TileHandler(Field &field) : FieldComponent(field)
    {}

    TileHandler::TileHandler(Field &field, TileHandler &&arg) : FieldComponent(field)
    {
        MoveContainer(std::move(arg.container));
    }

    TileHandler& TileHandler::operator=(TileHandler &&arg)
    {
        MoveContainer(std::move(arg.container));
        return *this;
    }

    void TileHandler::Work()
    {}

    Tile& TileHandler::Add(Tile &&tile)
    {
        return Add(std::move(tile), tile.GetPosition());
    }

    Tile& TileHandler::Add(Tile &&tile, const GridPosition &pos)
    {
        if (pos != tile.pos)
            tile.SetPosition(pos);

        Tile &placedTile = container.emplace(tile.GetPosition(), std::move(tile)).first->second;
        placedTile.handler = this;

        if (placedTile.GetPosition().z > maxZ)
            maxZ = placedTile.GetPosition().z;

        placedTile.PushSpritesToHandler();

        return placedTile;
    }

    void TileHandler::Delete(Tile &tile)
    {
        container.erase(tile.GetPosition());
    }

    void TileHandler::Delete(const GridPosition &pos)
    {
        container.erase(pos);
    }

    void TileHandler::Move(Tile &tile, const GridPosition &pos)
    {
        Move(tile.GetPosition(), pos);
    }

    void TileHandler::Move(Tile &tile, const RelativeGridPosition &offset)
    {
        Move(tile.GetPosition(), offset);
    }

    void TileHandler::Move(const GridPosition &here, const GridPosition &moveTo)
    {
        auto found = container.find(here);
        if (found == container.end())
            return;

        Tile hold(std::move(found->second));
        container.erase(found);
        hold.pos = moveTo;
        Add(std::move(hold));
    }

    void TileHandler::Move(const GridPosition &here, const RelativeGridPosition &offset)
    {
        Move(here, Atmos::GridPosition(here, offset));
    }

    Tile* TileHandler::Find(const GridPosition &pos)
    {
        auto found = container.find(pos);
        if (found != container.end())
            return &found->second;
        
        return nullptr;
    }

    const Tile* TileHandler::Find(const GridPosition &pos) const
    {
        auto found = container.find(pos);
        if (found != container.end())
            return &found->second;

        return nullptr;
    }

    bool TileHandler::Exists(const GridPosition &pos) const
    {
        return container.find(pos) != container.end();
    }

    void TileHandler::ToggleRoofs()
    {
        WorldManager::ToggleRoofs();
    }

    void TileHandler::SetRoofs(bool seen)
    {
        WorldManager::SetRoofs(seen);
    }

    GridPosition TileHandler::GetCenterOfTiles() const
    {
        if (container.empty())
            return GridPosition();

        GridPosition topLeft(container.begin()->first);
        GridPosition bottomRight(container.begin()->first);
        for (auto &loop : container)
        {
            if (loop.first.x > bottomRight.x)
                bottomRight.x = loop.first.x;
            else if (loop.first.x < topLeft.x)
                topLeft.x = loop.first.x;

            if (loop.first.y > bottomRight.y)
                bottomRight.y = loop.first.y;
            else if (loop.first.y < topLeft.y)
                topLeft.y = loop.first.y;

            if (loop.first.z > bottomRight.z)
                bottomRight.z = loop.first.z;
            else if (loop.first.z < topLeft.z)
                topLeft.z = loop.first.z;
        }

        return GridPosition(topLeft.x + ((bottomRight.x - topLeft.x) / 2), topLeft.y + ((bottomRight.y - topLeft.y) / 2));
    }

    GridPosition::ValueT TileHandler::GetMaxZ() const
    {
        return maxZ;
    }

    Tile* TileHandler::FindHighest(GridPosition::ValueT col, GridPosition::ValueT row)
    {
        GridPosition position(col, row, GetMaxZ());
        while (true)
        {
            auto found = Find(position);
            if (found)
                return found;

            if (position.z == 0)
                break;
            else
                --position.z;
        }

        return nullptr;
    }

    const Tile* TileHandler::FindHighest(GridPosition::ValueT col, GridPosition::ValueT row) const
    {
        GridPosition position(col, row, GetMaxZ());
        while (true)
        {
            auto found = Find(position);
            if (found)
                return found;

            if (position.z == 0)
                break;
            else
                --position.z;
        }

        return nullptr;
    }

    TileHandler::iterator TileHandler::begin()
    {
        return container.begin();
    }

    TileHandler::iterator TileHandler::end()
    {
        return container.end();
    }

    TileHandler::const_iterator TileHandler::begin() const
    {
        return container.begin();
    }

    TileHandler::const_iterator TileHandler::end() const
    {
        return container.end();
    }
}