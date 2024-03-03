
#pragma once

#include <stack>
#include <unordered_map>
#include <memory>

#include <Arca/Curator.h>

#include "GridPoint.h"
#include "BinaryHeap.h"

namespace Atmos::Spatial
{
    /*
    class Pathfinder final : public Arca::Curator
    {
    public:
        using Path = std::stack<Grid::Position>;
    public:
        Path FindPath(const Grid::Position& start, const Grid::Position& finish);
    private:
        class Node
        {
        public:
            using Cost = unsigned short;
        public:
            Node(Cost gCost, Cost hCost, const Grid::Position& position);
            Node(const Node& arg) = default;
            Node(Node&& arg) noexcept;

            Node& operator=(const Node& arg) = default;
            Node& operator=(Node&& arg) noexcept;

            bool operator>(const Node& arg) const;
            bool operator==(const Node& arg) const;
            bool operator!=(const Node& arg) const;

            [[nodiscard]] Grid::Position Position() const;

            void ChangeG(Cost to);
            void ChangeH(Cost to);

            [[nodiscard]] Cost G() const;
            [[nodiscard]] Cost H() const;
            [[nodiscard]] Cost F() const;

            void Reparent(const Node& setTo);
            [[nodiscard]] const Node* Parent() const;
        private:
            // Cost from frameStartTime along best path
            Cost gCost;
            // Heuristic
            Cost hCost;
            // Gcost + heuristic
            Cost fCost;
        private:
            Grid::Position position;
        private:
            using NodePtr = std::shared_ptr<Node>;
            NodePtr parent;
        };

        using NodeHeap = BinaryHeap<Node, std::greater<>>;
        using NodeMap = std::unordered_map<Grid::Position, Node>;
    private:
        NodeHeap::iterator FindNode(const Grid::Position& position, NodeHeap& heap) const;
        static void ReconstructPath(Path& stack, const Node& end);
        void ClearStack(Path& stack) const;
    };
    */
}