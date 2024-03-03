#pragma once

#include "FindPath.h"

#include <unordered_map>
#include <memory>
#include "BinaryHeap.h"

namespace Atmos::Entity
{
    class Pathfinder final
    {
    public:
        [[nodiscard]] Path FindPath(
            const Arca::Index<Entity>& entity,
            const Spatial::Grid::Point& to,
            Arca::Reliquary& reliquary);
    private:
        class Node
        {
        public:
            Spatial::Grid::Point position;
        public:
            using Cost = int;

            // Cost from frameStartTime along best path
            Cost gCost;
            // Heuristic
            Cost hCost;
            // Gcost + heuristic
            Cost fCost;
        public:
            using NodePtr = std::shared_ptr<Node>;
            NodePtr parent;
        public:
            Node(Cost gCost, Cost hCost, const Spatial::Grid::Point& position);
            Node(const Node& arg) = default;
            Node(Node&& arg) noexcept;

            Node& operator=(const Node& arg) = default;
            Node& operator=(Node&& arg) noexcept;

            bool operator==(const Node& arg) const;
            bool operator!=(const Node& arg) const;
            bool operator>(const Node& arg) const;
        };

        using NodeHeap = BinaryHeap<Node, std::greater<>>;
        using NodeMap = std::unordered_map<Spatial::Grid::Point, Node>;
    private:
        static inline const std::vector<Spatial::Grid::Point> relativeNeighbors =
        {
            Spatial::Grid::Point{-1, 0},
            Spatial::Grid::Point{0, -1},
            Spatial::Grid::Point{1, 0},
            Spatial::Grid::Point{0, 1}
        };

        [[nodiscard]] static NodeHeap::iterator FindNode(const Spatial::Grid::Point& position, NodeHeap& heap);
        [[nodiscard]] static Path ReconstructPath(const Node& end);
    };
}