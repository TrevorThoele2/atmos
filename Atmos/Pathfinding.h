
#pragma once

#include <stack>
#include <map>
#include <unordered_map>
#include <memory>

#include "Tile.h"
#include "GridPosition.h"
#include "BinaryHeap.h"

namespace Atmos
{
    class Pathfinder
    {
    public:
        typedef std::stack<GridPosition> TileStack;
    private:
        class Node
        {
        public:
            typedef unsigned short Cost;
        private:
            // Cost from start along best path
            Cost gCost;
            // Heuristic
            Cost hCost;
            // Gcost + heuristic
            Cost fCost;
            GridPosition pos;

            typedef std::shared_ptr<Node> NodePtr;
            NodePtr parent;
        public:
            Node(Cost gCost, Cost hCost, const GridPosition &pos);
            Node(const Node &arg);
            Node(Node &&arg);
            Node& operator=(const Node &arg);
            Node& operator=(Node &&arg);
            bool operator>(const Node &arg) const;
            bool operator==(const Node &arg) const;
            bool operator!=(const Node &arg) const;

            const GridPosition& GetPos() const;

            void SetG(Cost setTo);
            void SetH(Cost setTo);

            Cost GetG() const;
            Cost GetH() const;
            Cost GetF() const;

            void SetParent(const Node &setTo);
            const Node* GetParent() const;
        };

        typedef BinaryHeap<Node, std::greater<Node>> NodeHeap;
        typedef std::unordered_map<GridPosition, Node> NodeMap;
    private:
        Pathfinder() = default;
        Pathfinder(const Pathfinder &arg) = delete;
        Pathfinder& operator=(const Pathfinder &arg) = delete;

        static NodeHeap::iterator FindNode(const GridPosition& pos, NodeHeap &heap);
        static void ReconstructPath(TileStack &stack, const Node &end);
        static void ClearStack(TileStack &stack);
    public:
        static Pathfinder& Instance();
        static void FindPath(TileStack &stack, const GridPosition &start, const GridPosition &finish);
    };
}