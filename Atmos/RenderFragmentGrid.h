#pragma once

#include <array>
#include <unordered_set>
#include <memory>

#include "AxisBoundingBox3D.h"
#include "AxisBoundingBox2D.h"
#include "GridPosition.h"
#include "GridSize.h"
#include "Optional.h"

#include "ObjectReference.h"
#include "ObjectMap.h"

#include "PositionalObject.h"

namespace Atmos
{
    class Octree
    {
    public:
        typedef TypedObjectReference<PositionalObject> ItemReference;

        typedef ObjectMap<PositionalObject> Map;
        typedef size_t SizeT;
    public:
        Octree();
        Octree(const Octree& arg) = delete;
        Octree(Octree&& arg);
        Octree& operator=(const Octree& arg) = delete;
        Octree& operator=(Octree&& arg);

        void Work();

        void Add(ItemReference add);
        void Remove(ItemReference remove);
        void Clear();
        void InformChanged(ItemReference changed, const AxisBoundingBox3D& previousBounds);

        Map AllInside(const AxisBoundingBox3D& aabb);

        bool IsEmpty() const;
        SizeT Size() const;
    private:
        class NodeBase;
        typedef std::unique_ptr<NodeBase> NodePtr;

        // 0th layer is GRID_SIZE, 1st is GRID_SIZE * 2, 2nd is GRID_SIZE * 4
        typedef unsigned char Layer;

        typedef unsigned char ChildID;
        enum Child : ChildID
        {
            // If this order is ever changed, then the child selection in Emplace will need to be changed

            // West north far
            WNF = 0,
            // East north far
            ENF = 1,
            // West south far
            WSF = 2,
            // East south far
            ESF = 3,
            // West north near
            WNN = 4,
            // East north near
            ENN = 5,
            // West south near
            WSN = 6,
            // East south near
            ESN = 7
        };

        typedef std::array<NodePtr, 8> ChildrenArray;
    private:
        SizeT size;
        ChildrenArray heads;

        Map totalObjects;
        Map toAddObjects;

        // Will fabricate a LeafNode when layer == 0
        static NodeBase* FabricateNode(Layer layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
        void FabricateHeads(Layer layer);

        void AddImpl(ItemReference add);
        void RemoveImpl(ItemReference remove, const AxisBoundingBox3D& bounds);
        inline AxisBoundingBox3D GetTotalBounds() const
        {
            return AxisBoundingBox3D(
                heads[WNF]->GetBounds().GetLeft(),
                heads[WNF]->GetBounds().GetTop(),
                heads[WNF]->GetBounds().GetFarZ(),
                heads[ESN]->GetBounds().GetRight(),
                heads[ESN]->GetBounds().GetBottom(),
                heads[ESN]->GetBounds().GetNearZ());
        }

        inline bool IsWithinTotal(const AxisBoundingBox3D& bounds)
        {
            return bounds.Within(GetTotalBounds());
        }
    private:
        struct ChildDetermination
        {
            ChildID selection;
            GridPosition::ValueT xOffset;
            GridPosition::ValueT yOffset;
            GridPosition::ValueT zOffset;
            ChildDetermination(ChildID selection, GridPosition::ValueT xOffset, GridPosition::ValueT yOffset, GridPosition::ValueT zOffset);
        };

        // If the optional is invalid, that means that the box should be in this node
        static Optional<ChildDetermination> DetermineChild(GridPosition::ValueT layerSize, const AxisBoundingBox3D& bounds, const AxisBoundingBox3D& againstBounds);

        class NodeBase
        {
        protected:
            const GridPosition::ValueT x, y, z;
            Map objects;
        public:
            NodeBase(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
            NodeBase(const NodeBase& arg) = delete;
            NodeBase(NodeBase&& arg);
            NodeBase& operator=(const NodeBase& arg) = delete;
            NodeBase& operator=(NodeBase&& arg);
            virtual ~NodeBase() = 0;

            // Recursively will find a place for it; if it cannot find one, then it will return false
            virtual bool Emplace(ObjectReference emplace, const AxisBoundingBox3D& fragmentBounds) = 0;
            // Returns true if the fragment was removed successfully
            virtual bool Remove(ObjectReference remove, const AxisBoundingBox3D& fragmentBounds) = 0;

            virtual void Clear() = 0;

            virtual void AllInside(const AxisBoundingBox3D& aabb, Map& map) = 0;
            virtual bool IsTotalEmpty() const = 0;
            GridPosition GetPosition() const;
            virtual AxisBoundingBox3D GetBounds() const = 0;
            virtual Layer GetLayer() const = 0;
            virtual GridPosition::ValueT GetLayerSize() const = 0;
            // Returns the size of this set + the size of all the children
            virtual Map::SizeT GetTotalSize() const = 0;

            virtual bool IsLeaf() const = 0;
        };

        class LeafNode : public NodeBase
        {
        public:
            LeafNode(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
            LeafNode(const LeafNode& arg) = delete;
            LeafNode(LeafNode&& arg);
            LeafNode& operator=(const LeafNode& arg) = delete;
            LeafNode& operator=(LeafNode&& arg);

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(ObjectReference emplace, const AxisBoundingBox3D& fragmentBounds) override final;
            // Returns true if the fragment was removed successfully
            bool Remove(ObjectReference remove, const AxisBoundingBox3D& fragmentBounds) override final;

            void Clear() override final;

            void AllInside(const AxisBoundingBox3D& aabb, Map& map) override final;
            bool IsTotalEmpty() const override final;
            AxisBoundingBox3D GetBounds() const override final;
            Layer GetLayer() const override final;
            GridPosition::ValueT GetLayerSize() const override final;
            // Returns the size of this set + the size of all the children
            Map::SizeT GetTotalSize() const override final;

            bool IsLeaf() const override final { return true; }
        };

        class Node : public NodeBase
        {
        private:
            ChildrenArray children;
            AxisBoundingBox3D bounds;

            SizeT totalSize;

            Layer layer;
            GridPosition::ValueT layerSize;
        private:
            template<class ItrT>
            friend class IteratorBase;
            friend Octree;
        private:
            void DeleteChildren();
        public:
            Node(Layer layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
            Node(const Node& arg) = delete;
            Node(Node&& arg);
            Node& operator=(const Node& arg) = delete;
            Node& operator=(Node&& arg);

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(ObjectReference emplace, const AxisBoundingBox3D& fragmentBounds) override final;
            // Returns true if the fragment was removed successfully
            bool Remove(ObjectReference remove, const AxisBoundingBox3D& fragmentBounds) override final;

            // Returns true if the child was successfully added
            bool AddChild(NodeBase* node);

            void Clear() override final;

            void AllInside(const AxisBoundingBox3D& aabb, Map& map) override final;
            bool IsTotalEmpty() const override final;
            AxisBoundingBox3D GetBounds() const override final;
            Layer GetLayer() const override final;
            GridPosition::ValueT GetLayerSize() const override final;
            // Returns the size of this set + the size of all the children
            Map::SizeT GetTotalSize() const override final;

            bool IsLeaf() const override final { return false; }

            static GridPosition::ValueT FabricateLayerSize(Layer layer);
        };
    };
}