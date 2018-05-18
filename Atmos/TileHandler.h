
#pragma once

#include <unordered_map>

#include "FieldComponent.h"
#include "Tile.h"
#include "Serialization.h"

namespace Atmos
{
    class TileHandler : public FieldComponent
    {
    private:
        typedef std::unordered_map<GridPosition, Tile> Map;
    public:
        typedef Map::iterator iterator;
        typedef Map::const_iterator const_iterator;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        void Save(inscription::Scribe &scribe);
        void Load(inscription::Scribe &scribe);
        INSCRIPTION_ACCESS;
    private:
        Map container;

        GridPosition::ValueT maxZ;

        TileHandler(const TileHandler &arg) = delete;
        TileHandler& operator=(const TileHandler &arg) = delete;

        void MoveContainer(Map &&arg);
    public:
        TileHandler(Field &field);
        TileHandler(Field &field, TileHandler &&arg);
        TileHandler& operator=(TileHandler &&arg);

        void Work();

        template<class... Args>
        Tile& Create(Args && ... args);
        Tile& Add(Tile &&tile);
        Tile& Add(Tile &&tile, const GridPosition &pos);
        void Delete(Tile &tile);
        void Delete(const GridPosition &pos);
        void Move(Tile &tile, const GridPosition &pos);
        void Move(Tile &tile, const RelativeGridPosition &offset);
        void Move(const GridPosition &here, const GridPosition &moveTo);
        void Move(const GridPosition &here, const RelativeGridPosition &offset);

        Tile* Find(const GridPosition &pos);
        const Tile* Find(const GridPosition &pos) const;

        bool Exists(const GridPosition &pos) const;

        void ToggleRoofs();
        void SetRoofs(bool seen);

        // This is relatively expensive. Cache the result if possible
        GridPosition GetCenterOfTiles() const;
        GridPosition::ValueT GetMaxZ() const;
        Tile* FindHighest(GridPosition::ValueT col, GridPosition::ValueT row);
        const Tile* FindHighest(GridPosition::ValueT col, GridPosition::ValueT row) const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
    };

    template<class... Args>
    Tile& TileHandler::Create(Args && ... args)
    {
        return Add(Tile(std::forward<Args>(args)...));
    }
    // TileHandler
}