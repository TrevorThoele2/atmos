
#pragma once

#include <stack>
#include <map>
#include <unordered_map>
#include <memory>

#include "ObjectSystem.h"

#include "Tile.h"
#include "GridPosition.h"
#include "BinaryHeap.h"

namespace Atmos
{
    class Pathfinder : public ObjectSystem
    {
    public:
        typedef std::stack<GridPosition> TileStack;
    public:
        Pathfinder(ObjectManager& manager);

        TileStack FindPath(const GridPosition& start, const GridPosition& finish);
    private:
        class Node
        {
        public:
            typedef unsigned short Cost;
        public:
            Node(Cost gCost, Cost hCost, const GridPosition& position);
            Node(const Node& arg);
            Node(Node&& arg);

            Node& operator=(const Node& arg);
            Node& operator=(Node&& arg);

            bool operator>(const Node& arg) const;
            bool operator==(const Node& arg) const;
            bool operator!=(const Node& arg) const;

            const GridPosition& Position() const;

            void ChangeG(Cost to);
            void ChangeH(Cost to);

            Cost G() const;
            Cost H() const;
            Cost F() const;

            void Reparent(const Node& setTo);
            const Node* Parent() const;
        private:
            // Cost from start along best path
            Cost gCost;
            // Heuristic
            Cost hCost;
            // Gcost + heuristic
            Cost fCost;
        private:
            GridPosition position;
        private:
            typedef std::shared_ptr<Node> NodePtr;
            NodePtr parent;
        };

        typedef BinaryHeap<Node, std::greater<Node>> NodeHeap;
        typedef std::unordered_map<GridPosition, Node> NodeMap;
    private:
        NodeHeap::iterator FindNode(const GridPosition& position, NodeHeap& heap);
        void ReconstructPath(TileStack& stack, const Node& end);
        void ClearStack(TileStack& stack);
    };
}