#pragma once

#include <array>
#include <unordered_set>
#include <memory>

#include "AxisAlignedBox3D.h"
#include "GridPosition.h"
#include "TileSize.h"

namespace Atmos::Grid
{
    template<class T>
    class Octree
    {
    public:
        using List = std::vector<T*>;
    public:
        Octree();
        Octree(const Octree& arg) = delete;
        Octree(Octree&& arg) noexcept;
        Octree& operator=(const Octree& arg) = delete;
        Octree& operator=(Octree&& arg) noexcept;

        void Work();

        void Add(T& add);
        void Remove(T& remove);
        void Clear();
        void InformChanged(T& changed, const AxisAlignedBox3D& previousBounds);

        [[nodiscard]] List AllInside(const AxisAlignedBox3D& aabb) const;

        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] size_t Size() const;
    private:
        class Node;
        using NodePtr = std::unique_ptr<Node>;

        // 0th layer is TileSize, 1st is TileSize * 2, 2nd is TileSize * 4
        using Layer = unsigned char;

        using ChildID = unsigned char;
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

        using ChildrenArray = std::array<NodePtr, 8>;
    private:
        size_t size = 0;
        ChildrenArray heads;

        List totalObjects;
        List toAddObjects;

        // Will create a LeafNode when layer == 0
        static std::unique_ptr<Node> CreateNode(
            Layer layer,
            Position::Value layerSize,
            Position::Value x,
            Position::Value y,
            Position::Value z);
        void FabricateHeads(Layer layer);

        void AddImpl(T& add);
        void RemoveImpl(T& remove, const AxisAlignedBox3D& bounds);

        [[nodiscard]] AxisAlignedBox3D TotalBounds() const
        {
            return AxisAlignedBox3D
            {
                heads[WNF]->GetBounds().Left(),
                heads[WNF]->GetBounds().Top(),
                heads[WNF]->GetBounds().FarZ(),
                heads[ESN]->GetBounds().Right(),
                heads[ESN]->GetBounds().Bottom(),
                heads[ESN]->GetBounds().NearZ()
            };
        }

        [[nodiscard]] bool IsWithinTotal(const AxisAlignedBox3D& bounds) const
        {
            return bounds.Contains(TotalBounds());
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
        static std::optional<ChildDetermination> DetermineChild(
            Position::Value layerSize,
            const AxisAlignedBox3D& bounds,
            const AxisAlignedBox3D& againstBounds);

        class Node
        {
        public:
            Node(Position::Value x, Position::Value y, Position::Value z);
            Node(const Node& arg) = delete;
            Node(Node&& arg) noexcept;
            Node& operator=(const Node& arg) = delete;
            Node& operator=(Node&& arg) noexcept;
            virtual ~Node() = 0;

            // Recursively will find a place for it; if it cannot find one, then it will return false
            virtual bool Emplace(T& emplace, const AxisAlignedBox3D& fragmentBounds) = 0;
            // Returns true if the fragment was removed successfully
            virtual bool Remove(T& remove, const AxisAlignedBox3D& fragmentBounds) = 0;

            virtual void Clear() = 0;

            [[nodiscard]] virtual void AllInside(const AxisAlignedBox3D& aabb, List& list) = 0;
            [[nodiscard]] virtual bool IsTotalEmpty() const = 0;
            [[nodiscard]] Position Position() const;
            [[nodiscard]] virtual AxisAlignedBox3D Bounds() const = 0;
            [[nodiscard]] virtual Layer Layer() const = 0;
            [[nodiscard]] virtual Position::Value LayerSize() const = 0;
            // Returns the size of this set + the size of all the children
            [[nodiscard]] virtual size_t TotalSize() const = 0;

            [[nodiscard]] virtual bool IsLeaf() const = 0;
        protected:
            const Position::Value x, y, z;
            List objects;
        };

        class LeafNode final : public Node
        {
        public:
            LeafNode(Position::Value x, Position::Value y, Position::Value z);
            LeafNode(const LeafNode& arg) = delete;
            LeafNode(LeafNode&& arg) noexcept;
            LeafNode& operator=(const LeafNode& arg) = delete;
            LeafNode& operator=(LeafNode&& arg) noexcept;

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(T& emplace, const AxisAlignedBox3D& fragmentBounds) override;
            // Returns true if the fragment was removed successfully
            bool Remove(T& remove, const AxisAlignedBox3D& fragmentBounds) override;

            void Clear() override;

            void AllInside(const AxisAlignedBox3D& aabb, List& list) override;
            [[nodiscard]] bool IsTotalEmpty() const override;
            [[nodiscard]] AxisAlignedBox3D Bounds() const override;
            [[nodiscard]] Layer Layer() const override;
            [[nodiscard]] Position::Value LayerSize() const override;
            // Returns the size of this set + the size of all the children
            [[nodiscard]] size_t TotalSize() const override;

            [[nodiscard]] bool IsLeaf() const override
            {
                return true;
            }
        protected:
            using Node::x;
            using Node::y;
            using Node::z;
            using Node::objects;
        };

        class ParentNode final : public Node
        {
        private:
            ChildrenArray children;
            AxisAlignedBox3D bounds;

            size_t totalSize = 0;

            Layer layer;
            Position::Value layerSize;
        private:
            template<class ItrT>
            friend class IteratorBase;
            friend Octree;
        private:
            void DeleteChildren();
        public:
            ParentNode(
                Layer layer,
                Position::Value layerSize,
                Position::Value x,
                Position::Value y,
                Position::Value z);
            ParentNode(const ParentNode& arg) = delete;
            ParentNode(ParentNode&& arg) noexcept;
            ParentNode& operator=(const ParentNode& arg) = delete;
            ParentNode& operator=(ParentNode&& arg) noexcept;

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(T& emplace, const AxisAlignedBox3D& fragmentBounds) override;
            // Returns true if the fragment was removed successfully
            bool Remove(T& remove, const AxisAlignedBox3D& fragmentBounds) override;

            // Returns true if the child was successfully added
            bool AddChild(Node& node);

            void Clear() override;

            void AllInside(const AxisAlignedBox3D& aabb, List& list) override;
            [[nodiscard]] bool IsTotalEmpty() const override;
            [[nodiscard]] AxisAlignedBox3D Bounds() const override;
            [[nodiscard]] Layer Layer() const override;
            [[nodiscard]] Position::Value LayerSize() const override;
            // Returns the size of this set + the size of all the children
            [[nodiscard]] size_t TotalSize() const override;

            [[nodiscard]] bool IsLeaf() const override
            {
                return false;
            }

            static Position::Value FabricateLayerSize(typename Octree::Layer layer);
        protected:
            using Node::x;
            using Node::y;
            using Node::z;
            using Node::objects;
        };
    };

    template<class T>
    Octree<T>::Octree()
    {
        FabricateHeads(0);
    }

    template<class T>
    Octree<T>::Octree(Octree&& arg) noexcept :
        size(std::move(arg.size)), heads(std::move(arg.heads)),
        totalObjects(std::move(arg.totalObjects)), toAddObjects(std::move(arg.toAddObjects))
    {}

    template<class T>
    Octree<T>& Octree<T>::operator=(Octree&& arg) noexcept
    {
        size = std::move(arg.size);
        heads = std::move(arg.heads);
        totalObjects = std::move(arg.totalObjects);
        toAddObjects = std::move(arg.toAddObjects);
        return *this;
    }

    template<class T>
    void Octree<T>::Work()
    {
        if (totalObjects.IsEmpty())
            return;

        for (auto& loop : toAddObjects)
            AddImpl(loop);

        toAddObjects.Clear();
    }

    template<class T>
    void Octree<T>::Add(T& add)
    {
        if (toAddObjects.Has(add))
            return;

        RemoveImpl(add, add->Bounds());
        toAddObjects.Add(add);
    }

    template<class T>
    void Octree<T>::Remove(T& remove)
    {
        RemoveImpl(remove, remove->Bounds());
    }

    template<class T>
    void Octree<T>::Clear()
    {
        FabricateHeads(0);
        size = 0;
        totalObjects.Clear();
        toAddObjects.Clear();
    }

    template<class T>
    void Octree<T>::InformChanged(T& changed, const AxisAlignedBox3D& previousBounds)
    {
        if (toAddObjects.Has(changed))
            return;

        RemoveImpl(changed, previousBounds);
        toAddObjects.Add(changed);
    }

    template<class T>
    auto Octree<T>::AllInside(const AxisAlignedBox3D& aabb) const -> List
    {
        List returnValue;
        for (auto& loop : heads)
            loop->AllInside(aabb, returnValue);

        if (TotalBounds().Intersects(aabb))
            returnValue.Insert(totalObjects.begin(), totalObjects.end());

        return returnValue;
    }

    template<class T>
    bool Octree<T>::IsEmpty() const
    {
        return Size() == 0;
    }

    template<class T>
    size_t Octree<T>::Size() const
    {
        return size;
    }

    template<class T>
    auto Octree<T>::CreateNode(
        Layer layer,
        Position::Value layerSize,
        Position::Value x,
        Position::Value y,
        Position::Value z)

        -> std::unique_ptr<Node>
    {
        return layer == 0
            ? std::make_unique<LeafNode>(x, y, z)
            : std::make_unique<ParentNode>(layer, layerSize, x, y, z);
    }

    template<class T>
    void Octree<T>::FabricateHeads(Layer layer)
    {
        if (layer == 0)
        {
            heads[WNF] = std::make_unique<LeafNode>(-1, -1, -1);
            heads[ENF] = std::make_unique<LeafNode>(0, -1, -1);
            heads[WSF] = std::make_unique<LeafNode>(-1, 0, -1);
            heads[ESF] = std::make_unique<LeafNode>(0, 0, -1);
            heads[WNN] = std::make_unique<LeafNode>(-1, -1, 0);
            heads[ENN] = std::make_unique<LeafNode>(0, -1, 0);
            heads[WSN] = std::make_unique<LeafNode>(-1, 0, 0);
            heads[ESN] = std::make_unique<LeafNode>(0, 0, 0);
        }
        else
        {
            auto headCreator = [&](
                NodePtr& ptr,
                Position::Value x,
                Position::Value y,
                Position::Value z)
            {
                auto prevHead = ptr.release();
                ptr = std::make_unique<ParentNode>(layer, ParentNode::FabricateLayerSize(layer), x, y, z);
                if (prevHead->IsTotalEmpty())
                    delete prevHead;
                else
                {
                    if (!static_cast<ParentNode*>(ptr.get())->AddChild(prevHead))
                    {
                        size -= prevHead->GetTotalSize();
                        delete prevHead;
                    }
                }
            };

            headCreator(heads[WNF], -1, -1, -1);
            headCreator(heads[ENF], 0, -1, -1);
            headCreator(heads[WSF], -1, 0, -1);
            headCreator(heads[ESF], 0, 0, -1);
            headCreator(heads[WNN], -1, -1, 0);
            headCreator(heads[ENN], 0, -1, 0);
            headCreator(heads[WSN], -1, 0, 0);
            headCreator(heads[ESN], 0, 0, 0);
        }
    }

    template<class T>
    void Octree<T>::AddImpl(T& add)
    {
        auto attemptEmplace = []() -> bool
        {
            for (auto& loop : heads)
            {
                if (loop->Emplace(add, add->Bounds()))
                {
                    ++size;
                    return true;
                }
            }

            // Check if inside total
            if (IsWithinTotal(add->Bounds()))
            {
                totalObjects.Add(add);
                ++size;
                return true;
            }

            return false;
        };

        while (!attemptEmplace())
            FabricateHeads(heads[0]->GetLayer() + 1);
    }

    template<class T>
    void Octree<T>::RemoveImpl(T& remove, const AxisAlignedBox3D& bounds)
    {
        toAddObjects.Remove(remove);
        if (IsEmpty())
            return;

        for (auto& loop : heads)
        {
            if (loop->Remove(remove, bounds))
            {
                --size;
                return;
            }
        }

        if (IsWithinTotal(bounds) && totalObjects.Remove(remove))
            --size;
    }

    template<class T>
    Octree<T>::ChildDetermination::ChildDetermination(
        ChildID selection,
        Position::Value xOffset,
        Position::Value yOffset,
        Position::Value zOffset) :

        selection(selection), xOffset(xOffset), yOffset(yOffset), zOffset(zOffset)
    {}

    template<class T>
    auto Octree<T>::DetermineChild(
        Position::Value layerSize,
        const AxisAlignedBox3D& bounds,
        const AxisAlignedBox3D& againstBounds)

        -> std::optional<ChildDetermination>
    {
        const auto midpointX = bounds.Left() + (layerSize / 2.0f);
        const auto midpointY = bounds.Top() + (layerSize / 2.0f);
        const auto midpointZ = bounds.FarZ() + (layerSize / 2.0f);

        ChildDetermination determination(0, 0, 0, 0);

        // EAST = +0
        if (againstBounds.Left() >= midpointX)
        {
            // WEST = +1
            determination.selection += 1;
            determination.xOffset = 1;
        }
        else if (againstBounds.Right() > midpointX)
        {
            // The right is > midpointX and left < midpointX,
            //     which means that this crosses between two grids - cannot be divided further
            // So, must be here
            return {};
        }

        // NORTH = +0
        if (againstBounds.Top() >= midpointY)
        {
            // SOUTH = +2
            determination.selection += 2;
            determination.yOffset = 1;
        }
        else if (againstBounds.Bottom() > midpointY)
        {
            // The bottom is > midpointY and top < midpointY,
            //     which means that this crosses between two grids - cannot be divided further
            // So, must be here
            return {};
        }

        // FAR = +0
        if (againstBounds.FarZ() >= midpointZ)
        {
            // NEAR = +4
            determination.selection += 4;
            determination.zOffset = 1;
        }
        else if (againstBounds.NearZ() > midpointZ)
        {
            // The nearZ is > midpointZ and farZ < midpointZ,
            //     which means that this crosses between two grids - cannot be divided further
            // So, must be here
            return {};
        }

        return std::move(determination);
    }

    template<class T>
    Octree<T>::Node::Node(Position::Value x, Position::Value y, Position::Value z) :
        x(x), y(y), z(z)
    {}

    template<class T>
    Octree<T>::Node::Node(Node&& arg) noexcept : x(std::move(arg.x)), y(std::move(arg.y)), z(std::move(arg.z))
    {}

    template<class T>
    auto Octree<T>::Node::operator=(Node&& arg) noexcept -> Node&
    {
        const_cast<Position::Value&>(x) = std::move(arg.x);
        const_cast<Position::Value&>(y) = std::move(arg.y);
        const_cast<Position::Value&>(z) = std::move(arg.z);
        return *this;
    }

    template<class T>
    Octree<T>::Node::~Node() = default;

    template<class T>
    Position Octree<T>::Node::Position() const
    {
        return { x, y, z };
    }

    template<class T>
    Octree<T>::LeafNode::LeafNode(Position::Value x, Position::Value y, Position::Value z) :
        Node(x, y, z)
    {}

    template<class T>
    Octree<T>::LeafNode::LeafNode(LeafNode&& arg) noexcept : Node(std::move(arg))
    {}

    template<class T>
    auto Octree<T>::LeafNode::operator=(LeafNode&& arg) noexcept -> LeafNode&
    {
        Node::operator=(std::move(arg));
        return *this;
    }

    template<class T>
    bool Octree<T>::LeafNode::Emplace(T& emplace, const AxisAlignedBox3D& fragmentBounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Contains(Bounds()))
            return false;

        objects.Add(emplace);
        return true;
    }

    template<class T>
    bool Octree<T>::LeafNode::Remove(T& remove, const AxisAlignedBox3D& fragmentBounds)
    {
        return objects.Remove(remove);
    }

    template<class T>
    void Octree<T>::LeafNode::Clear()
    {
        objects.Clear();
    }

    template<class T>
    void Octree<T>::LeafNode::AllInside(const AxisAlignedBox3D& aabb, List& list)
    {
        // Check if the aabb overlaps this node
        if (!aabb.Intersects(Bounds()))
            return;

        list.insert(objects.begin(), objects.end());
    }

    template<class T>
    bool Octree<T>::LeafNode::IsTotalEmpty() const
    {
        return objects.IsEmpty();
    }

    template<class T>
    AxisAlignedBox3D Octree<T>::LeafNode::Bounds() const
    {
        const auto gridSize(TileSize<Position::Value>);
        return AxisAlignedBox3D(
            static_cast<AxisAlignedBox3D::Coordinate>(x * gridSize),
            static_cast<AxisAlignedBox3D::Coordinate>(y * gridSize),
            static_cast<AxisAlignedBox3D::Coordinate>(z * gridSize),
            static_cast<AxisAlignedBox3D::Coordinate>(x * gridSize + gridSize),
            static_cast<AxisAlignedBox3D::Coordinate>(y * gridSize + gridSize),
            static_cast<AxisAlignedBox3D::Coordinate>(z * gridSize + gridSize));
    }

    template<class T>
    auto Octree<T>::LeafNode::Layer() const -> typename Octree<T>::Layer
    {
        return 0;
    }

    template<class T>
    Position::Value Octree<T>::LeafNode::LayerSize() const
    {
        return TileSize<Position::Value>;
    }

    template<class T>
    size_t Octree<T>::LeafNode::TotalSize() const
    {
        return objects.Size();
    }

    template<class T>
    void Octree<T>::ParentNode::DeleteChildren()
    {
        for (auto& loop : children)
            loop.reset();
        totalSize = objects.Size();
    }

    template<class T>
    Octree<T>::ParentNode::ParentNode(
        typename Octree<T>::Layer layer,
        Position::Value layerSize,
        Position::Value x,
        Position::Value y,
        Position::Value z)
        :
        Node(x, y, z),
        bounds(
            static_cast<AxisAlignedBox3D::Coordinate>(x * layerSize),
            static_cast<AxisAlignedBox3D::Coordinate>(y * layerSize),
            static_cast<AxisAlignedBox3D::Coordinate>(z * layerSize),
            static_cast<AxisAlignedBox3D::Coordinate>(x * layerSize + layerSize),
            static_cast<AxisAlignedBox3D::Coordinate>(y * layerSize + layerSize),
            static_cast<AxisAlignedBox3D::Coordinate>(z * layerSize + layerSize)),
        layer(layer), layerSize(layerSize)
    {}

    template<class T>
    Octree<T>::ParentNode::ParentNode(ParentNode&& arg) noexcept :
        Node(std::move(arg)), children(std::move(arg.children)), bounds(std::move(arg.bounds)),
        totalSize(std::move(arg.totalSize)), layer(arg.layer), layerSize(arg.layerSize)
    {}

    template<class T>
    auto Octree<T>::ParentNode::operator=(ParentNode&& arg) noexcept -> ParentNode&
    {
        Node::operator=(std::move(arg));
        children = std::move(arg.children);
        bounds = std::move(arg.bounds);
        totalSize = std::move(arg.totalSize);
        layer = arg.layer;
        return *this;
    }

    template<class T>
    bool Octree<T>::ParentNode::Emplace(T& emplace, const AxisAlignedBox3D& fragmentBounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Contains(bounds))
            return false;

        // Try to emplace into the children
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            if (loop->Emplace(emplace, fragmentBounds))
            {
                ++totalSize;
                return true;
            }
        }

        // Wasn't emplaced into any children, but could require a new child

        auto determination = DetermineChild(layerSize, bounds, fragmentBounds);
        if (!determination.IsValid())
        {
            objects.Add(emplace);
            ++totalSize;
            return true;
        }

        NodePtr& selectedNode = children[determination->selection];
        DEBUG_ASSERT(!selectedNode);
        selectedNode.reset(CreateNode(
            layer - 1,
            FabricateLayerSize(layer - 1),
            (x * 2) + determination->xOffset,
            (y * 2) + determination->yOffset,
            (z * 2) + determination->zOffset));
        const bool emplaced = selectedNode->Emplace(emplace, fragmentBounds);
        if (emplaced)
            ++totalSize;

        return emplaced;
    }

    template<class T>
    bool Octree<T>::ParentNode::Remove(T& remove, const AxisAlignedBox3D& fragmentBounds)
    {
        // If this node is totally empty, then just leave
        if (IsTotalEmpty())
            return false;

        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Contains(bounds))
            return false;

        // Check all children to see if they can remove it
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            if (loop->Remove(remove, fragmentBounds))
            {
                // Delete the child node if it's empty
                if (loop->IsTotalEmpty())
                    loop.reset();

                --totalSize;
                return true;
            }
        }

        const bool erased = objects.Remove(remove);
        if (erased)
            --totalSize;
        return erased;
    }

    template<class T>
    bool Octree<T>::ParentNode::AddChild(Node& node)
    {
        auto determination = DetermineChild(layerSize, bounds, node->GetBounds());
        if (!determination)
            return false;

        children[determination->selection].reset(node);
        totalSize += node->GetTotalSize();
        return true;
    }

    template<class T>
    void Octree<T>::ParentNode::Clear()
    {
        DeleteChildren();
        objects.Clear();
        totalSize = 0;
    }

    template<class T>
    void Octree<T>::ParentNode::AllInside(const AxisAlignedBox3D& aabb, List& list)
    {
        // Check if the aabb overlaps this node
        if (!aabb.Intersects(bounds))
            return;

        // Insert from children
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            loop->AllInside(aabb, list);
        }

        // Insert fragments
        list.insert(objects.begin(), objects.end());
    }

    template<class T>
    bool Octree<T>::ParentNode::IsTotalEmpty() const
    {
        return totalSize == 0;
    }

    template<class T>
    AxisAlignedBox3D Octree<T>::ParentNode::Bounds() const
    {
        return bounds;
    }

    template<class T>
    auto Octree<T>::ParentNode::Layer() const -> typename Octree<T>::Layer
    {
        return layer;
    }

    template<class T>
    Position::Value Octree<T>::ParentNode::LayerSize() const
    {
        return layerSize;
    }

    template<class T>
    size_t Octree<T>::ParentNode::TotalSize() const
    {
        return totalSize;
    }

    template<class T>
    Position::Value Octree<T>::ParentNode::FabricateLayerSize(typename Octree<T>::Layer layer)
    {
        return TileSize<Position::Value> * PowerOfTwo(layer);
    }
}