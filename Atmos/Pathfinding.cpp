
#include "Pathfinding.h"

#include "TileSystem.h"
#include "CurrentField.h"

namespace Atmos
{
    Pathfinder::Node::Node(Cost gCost, Cost hCost, const GridPosition &pos) : gCost(gCost), hCost(hCost), pos(pos), fCost(gCost + hCost), parent(nullptr)
    {}

    Pathfinder::Node::Node(const Node &arg) : gCost(arg.gCost), hCost(arg.hCost), fCost(arg.fCost), pos(arg.pos), parent(arg.parent)
    {}

    Pathfinder::Node::Node(Node &&arg) : gCost(arg.gCost), hCost(arg.hCost), fCost(arg.fCost), pos(std::move(arg.pos)), parent(std::move(arg.parent))
    {}

    Pathfinder::Node& Pathfinder::Node::operator=(const Node &arg)
    {
        gCost = arg.gCost;
        hCost = arg.hCost;
        fCost = arg.fCost;
        pos = arg.pos;
        parent = arg.parent;
        return *this;
    }

    Pathfinder::Node& Pathfinder::Node::operator=(Node &&arg)
    {
        gCost = arg.gCost;
        hCost = arg.hCost;
        fCost = arg.fCost;
        pos = std::move(arg.pos);
        parent = std::move(arg.parent);
        return *this;
    }

    bool Pathfinder::Node::operator>(const Node &arg) const
    {
        return fCost > arg.fCost;
    }

    bool Pathfinder::Node::operator==(const Node &arg) const
    {
        return gCost == arg.gCost && hCost == arg.hCost && fCost == arg.fCost && parent == arg.parent;
    }

    bool Pathfinder::Node::operator!=(const Node &arg) const
    {
        return !(*this == arg);
    }

    const GridPosition& Pathfinder::Node::GetPos() const
    {
        return pos;
    }

    void Pathfinder::Node::SetG(Cost setTo)
    {
        gCost = setTo;
        fCost = gCost + hCost;
    }

    void Pathfinder::Node::SetH(Cost setTo)
    {
        hCost = setTo;
        fCost = gCost + hCost;
    }

    Pathfinder::Node::Cost Pathfinder::Node::GetG() const
    {
        return gCost;
    }

    Pathfinder::Node::Cost Pathfinder::Node::GetH() const
    {
        return hCost;
    }

    Pathfinder::Node::Cost Pathfinder::Node::GetF() const
    {
        return fCost;
    }

    void Pathfinder::Node::SetParent(const Pathfinder::Node &setTo)
    {
        parent.reset(new Node(setTo));
    }

    const Pathfinder::Node* Pathfinder::Node::GetParent() const
    {
        return parent.get();
    }

    Pathfinder::NodeHeap::iterator Pathfinder::FindNode(const GridPosition &pos, NodeHeap &heap)
    {
        for (auto loop = heap.begin(); loop != heap.end(); ++loop)
        {
            if (loop->GetPos() == pos)
                return loop;
        }

        return heap.end();
    }

    void Pathfinder::ReconstructPath(TileStack &stack, const Node &end)
    {
        stack.push(end.GetPos());
        auto parent = end.GetParent();
        while (parent->GetParent() != nullptr)
        {
            stack.push(parent->GetPos());
            parent = parent->GetParent();
        }
    }

    void Pathfinder::ClearStack(TileStack &stack)
    {
        // Empty the toReturn queue
        TileStack empty;
        std::swap(stack, empty);
    }

    Pathfinder& Pathfinder::Instance()
    {
        static Pathfinder instance;
        return instance;
    }

    void Pathfinder::FindPath(TileStack &stack, const GridPosition &start, const GridPosition &finish)
    {
        ClearStack(stack);
        if (start == finish)
            return;

        auto tiles = GetLocalObjectManager()->FindSystem<TileSystem>();
        if (!tiles)
            return;

        NodeHeap openSet;
        NodeMap closedSet;
        openSet.Push(Node(0, start.FindDistance(finish), start));

        while (!openSet.IsEmpty())
        {
            // Get current
            Node current = openSet.Top();
            if (current.GetPos() == finish)
                return ReconstructPath(stack, current);

            openSet.Pop();
            closedSet.emplace(current.GetPos(), current);

            // Find all neighbors
            for (int neighborLoop = DirectionIterationTraits::begin; neighborLoop <= DirectionIterationTraits::end; ++neighborLoop)
            {
                // Don't consider tile positions that don't exist or the tile is solid
                auto &neighborPos = current.GetPos().FindPositionAdjacent(Direction::FromUnderlyingType(neighborLoop));
                auto found = tiles->FindTile(neighborPos);
                if (neighborPos != finish && (!found || found->solid))
                    continue;

                Node neighbor(current.GetG() + 1, neighborPos.FindDistance(finish), neighborPos);

                if (closedSet.find(neighbor.GetPos()) != closedSet.end())
                    continue;

                // Find if node is in the open set
                NodeHeap::iterator neighborInOpen = FindNode(neighbor.GetPos(), openSet);

                if (neighborInOpen == openSet.end())
                {
                    neighbor.SetParent(current);
                    openSet.Push(neighbor);
                }
                else if (neighbor.GetG() < neighborInOpen->GetG())
                {
                    *neighborInOpen = neighbor;
                    neighborInOpen->SetParent(current);
                    openSet.Heapify();
                }
            }
        }

        // No path
    }
}