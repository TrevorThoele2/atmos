#pragma once

#include <array>
#include <unordered_set>
#include <memory>
#include "AxisBoundingBox3D.h"
#include "AxisBoundingBox2D.h"
#include "GridPosition.h"
#include "GridSize.h"
#include "Optional.h"

namespace Atmos
{
    class RenderFragment;
    class RenderFragmentGrid
    {
    public:
        typedef std::unordered_set<RenderFragment*> SetT;
        typedef size_t size_type;
    private:
        class NodeBase;
        typedef std::unique_ptr<NodeBase> NodePtr;

        // 0th layer is GRID_SIZE, 1st is GRID_SIZE * 2, 2nd is GRID_SIZE * 4
        typedef unsigned char LayerT;

        typedef unsigned char ChildT;
        enum Child : ChildT
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

        struct ChildDetermination
        {
            ChildT selection;
            GridPosition::ValueT xOffset;
            GridPosition::ValueT yOffset;
            GridPosition::ValueT zOffset;
            ChildDetermination(ChildT selection, GridPosition::ValueT xOffset, GridPosition::ValueT yOffset, GridPosition::ValueT zOffset);
        };

        // If the optional is invalid, that means that the box should be in this node
        static Optional<ChildDetermination> DetermineChild(GridPosition::ValueT layerSize, const AxisBoundingBox3D &bounds, const AxisBoundingBox3D &againstBounds);

        class NodeBase
        {
        protected:
            const GridPosition::ValueT x, y, z;
            SetT fragments;
        public:
            NodeBase(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
            NodeBase(const NodeBase &arg) = delete;
            NodeBase(NodeBase &&arg);
            NodeBase& operator=(const NodeBase &arg) = delete;
            NodeBase& operator=(NodeBase &&arg);
            virtual ~NodeBase() = 0 {}

            // Recursively will find a place for it; if it cannot find one, then it will return false
            virtual bool Emplace(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds) = 0;
            // Returns true if the fragment was removed successfully
            virtual bool Remove(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds) = 0;
        
            virtual void Clear() = 0;

            virtual void Find(const AxisBoundingBox3D &aabb, SetT &set) = 0;
            virtual bool IsTotalEmpty() const = 0;
            GridPosition GetPosition() const;
            virtual AxisBoundingBox3D GetBounds() const = 0;
            virtual LayerT GetLayer() const = 0;
            virtual GridPosition::ValueT GetLayerSize() const = 0;
            // Returns the size of this set + the size of all the children
            virtual SetT::size_type GetTotalSize() const = 0;

            virtual bool IsLeaf() const = 0;
        };

        class LeafNode : public NodeBase
        {
        public:
            LeafNode(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
            LeafNode(const LeafNode &arg) = delete;
            LeafNode(LeafNode &&arg);
            LeafNode& operator=(const LeafNode &arg) = delete;
            LeafNode& operator=(LeafNode &&arg);

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds) override final;
            // Returns true if the fragment was removed successfully
            bool Remove(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds) override final;

            void Clear() override final;

            void Find(const AxisBoundingBox3D &aabb, SetT &set) override final;
            bool IsTotalEmpty() const override final;
            AxisBoundingBox3D GetBounds() const override final;
            LayerT GetLayer() const override final;
            GridPosition::ValueT GetLayerSize() const override final;
            // Returns the size of this set + the size of all the children
            SetT::size_type GetTotalSize() const override final;

            bool IsLeaf() const override final { return true; }
        };

        class Node : public NodeBase
        {
        private:
            ChildrenArray children;
            AxisBoundingBox3D bounds;

            size_type totalSize;

            LayerT layer;
            GridPosition::ValueT layerSize;
        private:
            template<class ItrT>
            friend class IteratorBase;
            friend RenderFragmentGrid;
        private:
            void DeleteChildren();
        public:
            Node(LayerT layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
            Node(const Node &arg) = delete;
            Node(Node &&arg);
            Node& operator=(const Node &arg) = delete;
            Node& operator=(Node &&arg);

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds) override final;
            // Returns true if the fragment was removed successfully
            bool Remove(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds) override final;

            // Returns true if the child was successfully added
            bool AddChild(NodeBase *node);

            void Clear() override final;

            void Find(const AxisBoundingBox3D &aabb, SetT &set) override final;
            bool IsTotalEmpty() const override final;
            AxisBoundingBox3D GetBounds() const override final;
            LayerT GetLayer() const override final;
            GridPosition::ValueT GetLayerSize() const override final;
            // Returns the size of this set + the size of all the children
            SetT::size_type GetTotalSize() const override final;

            bool IsLeaf() const override final { return false; }

            static GridPosition::ValueT FabricateLayerSize(LayerT layer);
        };
    private:
        size_t size;
        ChildrenArray heads;

        SetT fragmentsInTotal;
        SetT fragmentsToAdd;

        // Will fabricate a LeafNode when layer == 0
        static NodeBase* FabricateNode(LayerT layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z);
        void FabricateHeads(LayerT layer);

        void AddImpl(RenderFragment &add);
        void RemoveImpl(RenderFragment &remove, const AxisBoundingBox3D &bounds);
        inline AxisBoundingBox3D GetTotalBounds() const
        {
            return AxisBoundingBox3D(heads[WNF]->GetBounds().GetLeft(),
                heads[WNF]->GetBounds().GetTop(),
                heads[WNF]->GetBounds().GetFarZ(),
                heads[ESN]->GetBounds().GetRight(),
                heads[ESN]->GetBounds().GetBottom(),
                heads[ESN]->GetBounds().GetNearZ());
        }

        inline bool IsWithinTotal(const AxisBoundingBox3D &bounds)
        {
            return bounds.Within(GetTotalBounds());
        }
    public:
        RenderFragmentGrid();
        RenderFragmentGrid(const RenderFragmentGrid &arg) = delete;
        RenderFragmentGrid(RenderFragmentGrid &&arg);
        RenderFragmentGrid& operator=(const RenderFragmentGrid &arg) = delete;
        RenderFragmentGrid& operator=(RenderFragmentGrid &&arg);

        void Work();

        void Add(RenderFragment &add);
        void Remove(RenderFragment &remove);
        void Clear();
        void InformChanged(RenderFragment &changed, const AxisBoundingBox3D &previousBounds);

        // Will return all of the fragments inside of the box
        SetT Find(const AxisBoundingBox3D &aabb);

        bool IsEmpty() const;
        size_type Size() const;
    };
}