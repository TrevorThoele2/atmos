
#include "Pathfinder.h"

#include "TileSystem.h"

namespace Atmos
{
    Pathfinder::Pathfinder(ObjectManager& manager) : ObjectSystem(manager)
    {}

    Pathfinder::TileStack Pathfinder::FindPath(const GridPosition& start, const GridPosition& finish)
    {
        TileStack createdStack;
        if (start == finish)
            return createdStack;

        auto tiles = Manager()->FindSystem<TileSystem>();
        if (!tiles)
            return createdStack;

        NodeHeap openSet;
        NodeMap closedSet;
        openSet.Push(Node(0, start.FindDistance(finish), start));

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
                auto& neighborPosition = current.Position().FindPositionAdjacent(Direction::FromUnderlyingType(neighborLoop));
                auto foundTile = tiles->FindTile(neighborPosition);
                if (neighborPosition != finish && (!foundTile || foundTile->solid))
                    continue;

                Node neighbor(current.G() + 1, neighborPosition.FindDistance(finish), neighborPosition);

                if (closedSet.find(neighbor.Position()) != closedSet.end())
                    continue;

                // Find if node is in the open set
                NodeHeap::iterator neighborInOpen = FindNode(neighbor.Position(), openSet);

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

    Pathfinder::Node::Node(Cost gCost, Cost hCost, const GridPosition& position) :
        gCost(gCost), hCost(hCost), position(position), fCost(gCost + hCost), parent(nullptr)
    {}

    Pathfinder::Node::Node(const Node& arg) :
        gCost(arg.gCost), hCost(arg.hCost), fCost(arg.fCost), position(arg.position), parent(arg.parent)
    {}

    Pathfinder::Node::Node(Node&& arg) :
        gCost(arg.gCost), hCost(arg.hCost), fCost(arg.fCost), position(std::move(arg.position)), parent(std::move(arg.parent))
    {}

    Pathfinder::Node& Pathfinder::Node::operator=(const Node& arg)
    {
        gCost = arg.gCost;
        hCost = arg.hCost;
        fCost = arg.fCost;
        position = arg.position;
        parent = arg.parent;
        return *this;
    }

    Pathfinder::Node& Pathfinder::Node::operator=(Node&& arg)
    {
        gCost = arg.gCost;
        hCost = arg.hCost;
        fCost = arg.fCost;
        position = std::move(arg.position);
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

    const GridPosition& Pathfinder::Node::Position() const
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

    void Pathfinder::Node::Reparent(const Pathfinder::Node& setTo)
    {
        parent.reset(new Node(setTo));
    }

    const Pathfinder::Node* Pathfinder::Node::Parent() const
    {
        return parent.get();
    }

    Pathfinder::NodeHeap::iterator Pathfinder::FindNode(const GridPosition& position, NodeHeap& heap)
    {
        return std::find_if(heap.begin(), heap.end(),
            [position](const Node& node)
        {
            return node.Position() == position;
        });
    }

    void Pathfinder::ReconstructPath(TileStack& stack, const Node& end)
    {
        stack.push(end.Position());
        auto parent = end.Parent();
        while (parent->Parent() != nullptr)
        {
            stack.push(parent->Position());
            parent = parent->Parent();
        }
    }

    void Pathfinder::ClearStack(TileStack& stack)
    {
        TileStack empty;
        std::swap(stack, empty);
    }
}