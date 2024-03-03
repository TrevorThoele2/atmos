#include "Pathfinder.h"

#include "Map.h"

namespace Atmos::Spatial
{
    /*
    Pathfinder::Path Pathfinder::FindPath(const Grid::Position& start, const Grid::Position& finish)
    {
        Path createdStack;
        if (start == finish)
            return createdStack;

        const auto map = Arca::Index<World::Map>(Owner());
        if (!map)
            return createdStack;

        NodeHeap openSet;
        NodeMap closedSet;
        openSet.Push(Node(0, Length(start, finish), start));

        while (!openSet.IsEmpty())
        {
            // Get current
            Node current = openSet.Top();
            if (current.Position() == finish)
            {
                ReconstructPath(createdStack, current);
                return createdStack;
            }

            openSet.Pop();
            closedSet.emplace(current.Position(), current);

            // Find all neighbors
            for (int neighborLoop = DirectionIterationTraits::begin; neighborLoop <= DirectionIterationTraits::end; ++neighborLoop)
            {
                // Don't consider tile positions that don't exist or the tile is solid
                auto neighborPosition = current.Position().FindPositionAdjacent(Direction::FromUnderlyingType(neighborLoop));
                const auto foundTile = map->entityRegion.find(neighborPosition);
                if (neighborPosition != finish && foundTile == map->entityRegion.end())
                    continue;

                Node neighbor(current.G() + 1, neighborPosition.FindDistance(finish), neighborPosition);

                if (closedSet.find(neighbor.Position()) != closedSet.end())
                    continue;

                // Find if node is in the open set
                const auto neighborInOpen = FindNode(neighbor.Position(), openSet);

                if (neighborInOpen == openSet.end())
                {
                    neighbor.Reparent(current);
                    openSet.Push(neighbor);
                }
                else if (neighbor.G() < neighborInOpen->G())
                {
                    *neighborInOpen = neighbor;
                    neighborInOpen->Reparent(current);
                    openSet.Heapify();
                }
            }
        }

        // No path
        return createdStack;
    }

    Pathfinder::Node::Node(Cost gCost, Cost hCost, const Grid::Position& position) :
        gCost(gCost), hCost(hCost), fCost(gCost + hCost),
        position(position), parent(nullptr)
    {}

    Pathfinder::Node::Node(Node&& arg) noexcept :
        gCost(arg.gCost), hCost(arg.hCost), fCost(arg.fCost),
        position(arg.position), parent(std::move(arg.parent))
    {}

    Pathfinder::Node& Pathfinder::Node::operator=(Node&& arg) noexcept
    {
        gCost = arg.gCost;
        hCost = arg.hCost;
        fCost = arg.fCost;
        position = arg.position;
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

    Grid::Position Pathfinder::Node::Position() const
    {
        return position;
    }

    void Pathfinder::Node::ChangeG(Cost to)
    {
        gCost = to;
        fCost = gCost + hCost;
    }

    void Pathfinder::Node::ChangeH(Cost to)
    {
        hCost = to;
        fCost = gCost + hCost;
    }

    Pathfinder::Node::Cost Pathfinder::Node::G() const
    {
        return gCost;
    }

    Pathfinder::Node::Cost Pathfinder::Node::H() const
    {
        return hCost;
    }

    Pathfinder::Node::Cost Pathfinder::Node::F() const
    {
        return fCost;
    }

    void Pathfinder::Node::Reparent(const Node& setTo)
    {
        parent = std::make_unique<Node>(setTo);
    }

    const Pathfinder::Node* Pathfinder::Node::Parent() const
    {
        return parent.get();
    }

    Pathfinder::NodeHeap::iterator Pathfinder::FindNode(const Grid::Position& position, NodeHeap& heap) const
    {
        return std::find_if(heap.begin(), heap.end(),
            [position](const Node& node)
        {
            return node.Position() == position;
        });
    }

    void Pathfinder::ReconstructPath(Path& stack, const Node& end)
    {
        stack.push(end.Position());
        auto parent = end.Parent();
        while (parent->Parent() != nullptr)
        {
            stack.push(parent->Position());
            parent = parent->Parent();
        }
    }

    void Pathfinder::ClearStack(Path& stack) const
    {
        Path empty;
        std::swap(stack, empty);
    }
    */
}