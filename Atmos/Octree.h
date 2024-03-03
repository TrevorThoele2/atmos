#pragma once

#include <array>
#include <unordered_set>
#include <memory>

#include "AxisAlignedBox3D.h"
#include "AxisAlignedBox2D.h"
#include "GridPosition.h"
#include "TileSize.h"
#include "Optional.h"

#include "ObjectReference.h"
#include "ObjectMap.h"

#include "AxisAlignedObject.h"

namespace Atmos::Grid
{
    class Octree
    {
    public:
        typedef TypedObjectReference<AxisAlignedObject> ItemReference;

        typedef ObjectMap<AxisAlignedObject> Map;
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
        void InformChanged(ItemReference changed, const AxisAlignedBox3D& previousBounds);

        Map AllInside(const AxisAlignedBox3D& aabb);

        bool IsEmpty() const;
        SizeT Size() const;
    private:
        class NodeBase;
        typedef std::unique_ptr<NodeBase> NodePtr;

        // 0th layer is TileSize, 1st is TileSize * 2, 2nd is TileSize * 4
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
        static NodeBase* FabricateNode(
            Layer layer,
            Position::Value layerSize,
            Position::Value x,
            Position::Value y,
            Position::Value z);
        void FabricateHeads(Layer layer);

        void AddImpl(ItemReference add);
        void RemoveImpl(ItemReference remove, const AxisAlignedBox3D& bounds);
        inline AxisAlignedBox3D GetTotalBounds() const
        {
            return AxisAlignedBox3D(
                heads[WNF]->GetBounds().left,
                heads[WNF]->GetBounds().top,
                heads[WNF]->GetBounds().farZ,
                heads[ESN]->GetBounds().right,
                heads[ESN]->GetBounds().bottom,
                heads[ESN]->GetBounds().nearZ);
        }

        inline bool IsWithinTotal(const AxisAlignedBox3D& bounds)
        {
            return bounds.Within(GetTotalBounds());
        }
    private:
        struct ChildDetermination
        {
            ChildID selection;
            Position::Value xOffset;
            Position::Value yOffset;
            Position::Value zOffset;
            ChildDetermination(
                ChildID selection,
                Position::Value xOffset,
                Position::Value yOffset,
                Position::Value zOffset);
        };

        // If the optional is invalid, that means that the box should be in this node
        static Optional<ChildDetermination> DetermineChild(
            Position::Value layerSize,
            const AxisAlignedBox3D& bounds,
            const AxisAlignedBox3D& againstBounds);

        class NodeBase
        {
        protected:
            const Position::Value x, y, z;
            Map objects;
        public:
            NodeBase(Position::Value x, Position::Value y, Position::Value z);
            NodeBase(const NodeBase& arg) = delete;
            NodeBase(NodeBase&& arg);
            NodeBase& operator=(const NodeBase& arg) = delete;
            NodeBase& operator=(NodeBase&& arg);
            virtual ~NodeBase() = 0;

            // Recursively will find a place for it; if it cannot find one, then it will return false
            virtual bool Emplace(ObjectReference emplace, const AxisAlignedBox3D& fragmentBounds) = 0;
            // Returns true if the fragment was removed successfully
            virtual bool Remove(ObjectReference remove, const AxisAlignedBox3D& fragmentBounds) = 0;

            virtual void Clear() = 0;

            virtual void AllInside(const AxisAlignedBox3D& aabb, Map& map) = 0;
            virtual bool IsTotalEmpty() const = 0;
            Position GetPosition() const;
            virtual AxisAlignedBox3D GetBounds() const = 0;
            virtual Layer GetLayer() const = 0;
            virtual Position::Value GetLayerSize() const = 0;
            // Returns the size of this set + the size of all the children
            virtual Map::SizeT GetTotalSize() const = 0;

            virtual bool IsLeaf() const = 0;
        };

        class LeafNode : public NodeBase
        {
        public:
            LeafNode(Position::Value x, Position::Value y, Position::Value z);
            LeafNode(const LeafNode& arg) = delete;
            LeafNode(LeafNode&& arg);
            LeafNode& operator=(const LeafNode& arg) = delete;
            LeafNode& operator=(LeafNode&& arg);

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(ObjectReference emplace, const AxisAlignedBox3D& fragmentBounds) override final;
            // Returns true if the fragment was removed successfully
            bool Remove(ObjectReference remove, const AxisAlignedBox3D& fragmentBounds) override final;

            void Clear() override final;

            void AllInside(const AxisAlignedBox3D& aabb, Map& map) override final;
            bool IsTotalEmpty() const override final;
            AxisAlignedBox3D GetBounds() const override final;
            Layer GetLayer() const override final;
            Position::Value GetLayerSize() const override final;
            // Returns the size of this set + the size of all the children
            Map::SizeT GetTotalSize() const override final;

            bool IsLeaf() const override final { return true; }
        };

        class Node : public NodeBase
        {
        private:
            ChildrenArray children;
            AxisAlignedBox3D bounds;

            SizeT totalSize;

            Layer layer;
            Position::Value layerSize;
        private:
            template<class ItrT>
            friend class IteratorBase;
            friend Octree;
        private:
            void DeleteChildren();
        public:
            Node(
                Layer layer,
                Position::Value layerSize,
                Position::Value x,
                Position::Value y,
                Position::Value z);
            Node(const Node& arg) = delete;
            Node(Node&& arg);
            Node& operator=(const Node& arg) = delete;
            Node& operator=(Node&& arg);

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(ObjectReference emplace, const AxisAlignedBox3D& fragmentBounds) override final;
            // Returns true if the fragment was removed successfully
            bool Remove(ObjectReference remove, const AxisAlignedBox3D& fragmentBounds) override final;

            // Returns true if the child was successfully added
            bool AddChild(NodeBase* node);

            void Clear() override final;

            void AllInside(const AxisAlignedBox3D& aabb, Map& map) override final;
            bool IsTotalEmpty() const override final;
            AxisAlignedBox3D GetBounds() const override final;
            Layer GetLayer() const override final;
            Position::Value GetLayerSize() const override final;
            // Returns the size of this set + the size of all the children
            Map::SizeT GetTotalSize() const override final;

            bool IsLeaf() const override final { return false; }

            static Position::Value FabricateLayerSize(Layer layer);
        };
    };
}