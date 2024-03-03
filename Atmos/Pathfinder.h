
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
        typedef std::stack<Grid::Position> TileStack;
    public:
        Pathfinder(ObjectManager& manager);

        TileStack FindPath(const Grid::Position& start, const Grid::Position& finish);
    private:
        class Node
        {
        public:
            typedef unsigned short Cost;
        public:
            Node(Cost gCost, Cost hCost, const Grid::Position& position);
            Node(const Node& arg);
            Node(Node&& arg);

            Node& operator=(const Node& arg);
            Node& operator=(Node&& arg);

            bool operator>(const Node& arg) const;
            bool operator==(const Node& arg) const;
            bool operator!=(const Node& arg) const;

            const Grid::Position& Position() const;

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
            Grid::Position position;
        private:
            typedef std::shared_ptr<Node> NodePtr;
            NodePtr parent;
        };

        typedef BinaryHeap<Node, std::greater<Node>> NodeHeap;
        typedef std::unordered_map<Grid::Position, Node> NodeMap;
    private:
        NodeHeap::iterator FindNode(const Grid::Position& position, NodeHeap& heap);
        void ReconstructPath(TileStack& stack, const Node& end);
        void ClearStack(TileStack& stack);
    };
}