#include "Pathfinder.h"

#include "SpatialAlgorithms.h"

#include "CanMoveEntityTo.h"
#include <Arca/Reliquary.h>

namespace Atmos::Entity
{
    Path Pathfinder::FindPath(
        const Arca::Index<Entity>& entity,
        const Spatial::Grid::Point& to,
        Arca::Reliquary& reliquary)
    {
        const auto from = entity->position;
        if (from == to)
            return {};

        Path currentPath;

        NodeHeap openSet;
        NodeMap closedSet;
        openSet.Push(Node(0, Distance(from, to), from));

        while (!openSet.IsEmpty())
        {
            auto current = openSet.Top();
            if (current.position == to)
            {
                ReconstructPath(currentPath, current);
                return currentPath;
            }

            openSet.Pop();
            closedSet.emplace(current.position, current);

            for (auto& relativeNeighbor : relativeNeighbors)
            {
                // Don't consider tile positions that don't exist or the tile is solid
                auto neighborPosition = Spatial::Grid::Point
                {
                    current.position.x + relativeNeighbor.x,
                    current.position.y + relativeNeighbor.y
                };
                const auto canMoveTo = reliquary.Do(CanMoveTo{ entity, to });
                if (neighborPosition != to && !canMoveTo)
                    continue;

                Node neighbor(current.gCost + 1, Distance(neighborPosition, to), neighborPosition);

                if (closedSet.find(neighbor.position) != closedSet.end())
                    continue;

                // Find if node is in the open set
                const auto neighborInOpen = FindNode(neighbor.position, openSet);

                if (neighborInOpen == openSet.end())
                {
                    neighbor.parent = std::make_shared<Node>(current);
                    openSet.Push(neighbor);
                }
                else if (neighbor.gCost < neighborInOpen->gCost)
                {
                    *neighborInOpen = neighbor;
                    neighborInOpen->parent = std::make_shared<Node>(current);
                    openSet.Heapify();
                }
            }
        }

        return {};
    }

    Pathfinder::Node::Node(Cost gCost, Cost hCost, const Spatial::Grid::Point& position) :
        position(position),
        gCost(gCost),
        hCost(hCost),
        fCost(gCost + hCost)
    {}

    Pathfinder::Node::Node(Node&& arg) noexcept :
        position(arg.position),
        gCost(arg.gCost),
        hCost(arg.hCost),
        fCost(arg.fCost),
        parent(std::move(arg.parent))
    {}

    Pathfinder::Node& Pathfinder::Node::operator=(Node&& arg) noexcept
    {
        position = arg.position;
        gCost = arg.gCost;
        hCost = arg.hCost;
        fCost = arg.fCost;
        parent = std::move(arg.parent);
        return *this;
    }

    bool Pathfinder::Node::operator>(const Node& arg) const
    {
        return fCost > arg.fCost;
    }

    bool Pathfinder::Node::operator==(const Node& arg) const
    {
        return gCost == arg.gCost && hCost == arg.hCost && fCost == arg.fCost && parent == arg.parent;
    }

    bool Pathfinder::Node::operator!=(const Node& arg) const
    {
        return !(*this == arg);
    }

    Pathfinder::NodeHeap::iterator Pathfinder::FindNode(const Spatial::Grid::Point& position, NodeHeap& heap)
    {
        return std::find_if(
            heap.begin(),
            heap.end(),
            [position](const Node& node)
            {
                return node.position == position;
            });
    }

    void Pathfinder::ReconstructPath(Path& stack, const Node& end)
    {
        stack.push_back(end.position);
        auto parent = end.parent.get();
        while (parent->parent != nullptr)
        {
            stack.push_back(parent->position);
            parent = parent->parent.get();
        }
    }
}