#pragma once

#include <forward_list>
#include <unordered_map>
#include <memory>
#include "GridPosition.h"

namespace Atmos
{
    class ApproximateLighting
    {
    public:
        class Node
        {
        public:
            RelativeGridPosition relPosition;
            std::forward_list<Node*> children;

            Node(const RelativeGridPosition &relPosition);
        };

        // Will only make the node if it's unique, otherwise will return the node at the relative position
        Node* MakeOrFindNode(const RelativeGridPosition &relPosition);
        // Makes a section of identical direction positions
        // If any of the radii are negative, then the section's coordinates will be considered to be negative
        void MakeSection(GridPosition::ValueT xRadius, GridPosition::ValueT yRadius, GridPosition::ValueT zRadius, float maximumLength);
    public:
        typedef std::unique_ptr<Node> NodePtr;
        std::unordered_map<RelativeGridPosition, NodePtr> nodes;

        Node *head;

        ApproximateLighting();

        // Sphere constructor
        void SolidifySphere(float radius, const Position3D &center);
    };
}