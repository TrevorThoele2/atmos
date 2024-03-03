#pragma once

#include <array>
#include <memory>
#include <assert.h>

#include "AxisAlignedBox3D.h"
#include "GridPosition.h"
#include "TileSize.h"
#include "MathUtility.h"

namespace Atmos::Grid
{
    template<class ID, class T>
    class Octree
    {
    public:
        struct Stored
        {
            ID id;
            T value;

            Stored(ID id, const T& value);
        };

        using List = std::vector<Stored>;
    public:
        Octree();
        Octree(const Octree& arg) = delete;
        Octree(Octree&& arg) noexcept;
        Octree& operator=(const Octree& arg) = delete;
        Octree& operator=(Octree&& arg) noexcept;

        void Add(ID id, const T& add, const AxisAlignedBox3D& bounds);
        void Remove(ID id, const AxisAlignedBox3D& bounds);
        void Clear();
        void InformChanged(
            ID id,
            const AxisAlignedBox3D& previousBounds,
            const AxisAlignedBox3D& newBounds);

        [[nodiscard]] List AllInside(const AxisAlignedBox3D& aabb) const;

        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] size_t Size() const;
    private:
        class Node;
        using NodePtr = std::unique_ptr<Node>;

        // 0th layer is TileSize, 1st is TileSize * 2, 2nd is TileSize * 4
        using LayerNumber = unsigned char;

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

        // Will create a LeafNode when layer == 0
        static std::unique_ptr<Node> CreateNode(
            LayerNumber layer,
            Position::Value layerSize,
            Position::Value x,
            Position::Value y,
            Position::Value z);
        void FabricateHeads(LayerNumber layer);

        [[nodiscard]] AxisAlignedBox3D TotalBounds() const
        {
            return AxisAlignedBox3D
            {
                heads[WNF]->Bounds().Left(),
                heads[WNF]->Bounds().Top(),
                heads[WNF]->Bounds().FarZ(),
                heads[ESN]->Bounds().Right(),
                heads[ESN]->Bounds().Bottom(),
                heads[ESN]->Bounds().NearZ()
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
            virtual bool Emplace(ID id, const T& emplace, const AxisAlignedBox3D& bounds) = 0;
            // Returns true if the fragment was removed successfully
            virtual bool Remove(ID id, const AxisAlignedBox3D& bounds) = 0;

            virtual void Clear() = 0;

            [[nodiscard]] virtual void AllInside(const AxisAlignedBox3D& aabb, List& list) = 0;
            [[nodiscard]] virtual bool IsTotalEmpty() const = 0;
            [[nodiscard]] Position Position() const;
            [[nodiscard]] virtual AxisAlignedBox3D Bounds() const = 0;
            [[nodiscard]] virtual LayerNumber Layer() const = 0;
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
            bool Emplace(ID id, const T& emplace, const AxisAlignedBox3D& bounds) override;
            // Returns true if the fragment was removed successfully
            bool Remove(ID id, const AxisAlignedBox3D& bounds) override;

            void Clear() override;

            void AllInside(const AxisAlignedBox3D& aabb, List& list) override;
            [[nodiscard]] bool IsTotalEmpty() const override;
            [[nodiscard]] AxisAlignedBox3D Bounds() const override;
            [[nodiscard]] LayerNumber Layer() const override;
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

            LayerNumber layer;
            Position::Value layerSize;
        private:
            template<class ItrT>
            friend class IteratorBase;
            friend Octree;
        private:
            void DeleteChildren();
        public:
            ParentNode(
                LayerNumber layer,
                Position::Value layerSize,
                Position::Value x,
                Position::Value y,
                Position::Value z);
            ParentNode(const ParentNode& arg) = delete;
            ParentNode(ParentNode&& arg) noexcept;
            ParentNode& operator=(const ParentNode& arg) = delete;
            ParentNode& operator=(ParentNode&& arg) noexcept;

            // Recursively will find a place for it; if it cannot find one, then it will return false
            bool Emplace(ID id, const T& emplace, const AxisAlignedBox3D& bounds) override;
            // Returns true if the fragment was removed successfully
            bool Remove(ID id, const AxisAlignedBox3D& bounds) override;

            // Returns true if the child was successfully added
            bool AddChild(Node& node);

            void Clear() override;

            void AllInside(const AxisAlignedBox3D& aabb, List& list) override;
            [[nodiscard]] bool IsTotalEmpty() const override;
            [[nodiscard]] AxisAlignedBox3D Bounds() const override;
            [[nodiscard]] LayerNumber Layer() const override;
            [[nodiscard]] Position::Value LayerSize() const override;
            // Returns the size of this set + the size of all the children
            [[nodiscard]] size_t TotalSize() const override;

            [[nodiscard]] bool IsLeaf() const override
            {
                return false;
            }

            static Position::Value FabricateLayerSize(LayerNumber layer);
        protected:
            using Node::x;
            using Node::y;
            using Node::z;
            using Node::objects;
        };
    };

    template<class ID, class T>
    Octree<ID, T>::Stored::Stored(ID id, const T& value) : id(id), value(value)
    {}

    template<class ID, class T>
    Octree<ID, T>::Octree()
    {
        FabricateHeads(0);
    }

    template<class ID, class T>
    Octree<ID, T>::Octree(Octree&& arg) noexcept :
        size(std::move(arg.size)), heads(std::move(arg.heads)),
        totalObjects(std::move(arg.totalObjects))
    {}

    template<class ID, class T>
    Octree<ID, T>& Octree<ID, T>::operator=(Octree&& arg) noexcept
    {
        size = std::move(arg.size);
        heads = std::move(arg.heads);
        totalObjects = std::move(arg.totalObjects);
        return *this;
    }

    template<class ID, class T>
    void Octree<ID, T>::Add(ID id, const T& add, const AxisAlignedBox3D& bounds)
    {
        auto attemptEmplace = [this, id, &add, bounds]() -> bool
        {
            for (auto& loop : heads)
            {
                if (loop->Emplace(id, add, bounds))
                {
                    ++size;
                    return true;
                }
            }

            // Check if inside total
            if (IsWithinTotal(bounds))
            {
                totalObjects.emplace_back(id, add);
                ++size;
                return true;
            }

            return false;
        };

        while (!attemptEmplace())
            FabricateHeads(heads[0]->Layer() + 1);
    }

    template<class ID, class T>
    void Octree<ID, T>::Remove(ID id, const AxisAlignedBox3D& bounds)
    {
        if (IsEmpty())
            return;

        for (auto& loop : heads)
        {
            if (loop->Remove(id, bounds))
            {
                --size;
                return;
            }
        }

        if (IsWithinTotal(bounds))
        {
            for(auto loop = totalObjects.begin(); loop != totalObjects.end(); ++loop)
            {
                if(loop->id == id)
                {
                    totalObjects.erase(loop);
                    --size;
                    break;
                }
            }
        }
    }

    template<class ID, class T>
    void Octree<ID, T>::Clear()
    {
        FabricateHeads(0);
        size = 0;
        totalObjects.Clear();
    }

    template<class ID, class T>
    void Octree<ID, T>::InformChanged(
        ID id,
        const AxisAlignedBox3D& previousBounds,
        const AxisAlignedBox3D& newBounds)
    {
        Remove(id, previousBounds);
        Add(id, newBounds);
    }

    template<class ID, class T>
    auto Octree<ID, T>::AllInside(const AxisAlignedBox3D& aabb) const -> List
    {
        List returnValue;
        for (auto& loop : heads)
            loop->AllInside(aabb, returnValue);

        if (TotalBounds().Intersects(aabb))
            returnValue.insert(returnValue.begin(), totalObjects.begin(), totalObjects.end());

        return returnValue;
    }

    template<class ID, class T>
    bool Octree<ID, T>::IsEmpty() const
    {
        return Size() == 0;
    }

    template<class ID, class T>
    size_t Octree<ID, T>::Size() const
    {
        return size;
    }

    template<class ID, class T>
    auto Octree<ID, T>::CreateNode(
        LayerNumber layer,
        Position::Value layerSize,
        Position::Value x,
        Position::Value y,
        Position::Value z)

        -> std::unique_ptr<Node>
    {
        return layer == 0
            ? static_cast<std::unique_ptr<Node>>(std::make_unique<LeafNode>(x, y, z))
            : static_cast<std::unique_ptr<Node>>(std::make_unique<ParentNode>(layer, layerSize, x, y, z));
    }

    template<class ID, class T>
    void Octree<ID, T>::FabricateHeads(LayerNumber layer)
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
            auto headCreator = [this, layer](
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
                    if (!static_cast<ParentNode*>(ptr.get())->AddChild(*prevHead))
                    {
                        size -= prevHead->TotalSize();
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

    template<class ID, class T>
    Octree<ID, T>::ChildDetermination::ChildDetermination(
        ChildID selection,
        Position::Value xOffset,
        Position::Value yOffset,
        Position::Value zOffset)
        :
        selection(selection), xOffset(xOffset), yOffset(yOffset), zOffset(zOffset)
    {}

    template<class ID, class T>
    auto Octree<ID, T>::DetermineChild(
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

    template<class ID, class T>
    Octree<ID, T>::Node::Node(Position::Value x, Position::Value y, Position::Value z) :
        x(x), y(y), z(z)
    {}

    template<class ID, class T>
    Octree<ID, T>::Node::Node(Node&& arg) noexcept :
        x(arg.x), y(arg.y), z(arg.z)
    {}

    template<class ID, class T>
    auto Octree<ID, T>::Node::operator=(Node&& arg) noexcept -> Node&
    {
        x = arg.x;
        y = arg.y;
        z = arg.z;
        return *this;
    }

    template<class ID, class T>
    Octree<ID, T>::Node::~Node() = default;

    template<class ID, class T>
    Position Octree<ID, T>::Node::Position() const
    {
        return { x, y, z };
    }

    template<class ID, class T>
    Octree<ID, T>::LeafNode::LeafNode(Position::Value x, Position::Value y, Position::Value z) :
        Node(x, y, z)
    {}

    template<class ID, class T>
    Octree<ID, T>::LeafNode::LeafNode(LeafNode&& arg) noexcept : Node(std::move(arg))
    {}

    template<class ID, class T>
    auto Octree<ID, T>::LeafNode::operator=(LeafNode&& arg) noexcept -> LeafNode&
    {
        Node::operator=(std::move(arg));
        return *this;
    }

    template<class ID, class T>
    bool Octree<ID, T>::LeafNode::Emplace(ID id, const T& emplace, const AxisAlignedBox3D& bounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!bounds.Contains(Bounds()))
            return false;

        objects.emplace_back(id, emplace);
        return true;
    }

    template<class ID, class T>
    bool Octree<ID, T>::LeafNode::Remove(ID id, const AxisAlignedBox3D& bounds)
    {
        for (auto loop = objects.begin(); loop != objects.end(); ++loop)
        {
            if (loop->id == id)
            {
                objects.erase(loop);
                return true;
            }
        }

        return false;
    }

    template<class ID, class T>
    void Octree<ID, T>::LeafNode::Clear()
    {
        objects.clear();
    }

    template<class ID, class T>
    void Octree<ID, T>::LeafNode::AllInside(const AxisAlignedBox3D& aabb, List& list)
    {
        if (!aabb.Intersects(Bounds()))
            return;

        list.insert(list.begin(), objects.begin(), objects.end());
    }

    template<class ID, class T>
    bool Octree<ID, T>::LeafNode::IsTotalEmpty() const
    {
        return objects.empty();
    }

    template<class ID, class T>
    AxisAlignedBox3D Octree<ID, T>::LeafNode::Bounds() const
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

    template<class ID, class T>
    auto Octree<ID, T>::LeafNode::Layer() const -> LayerNumber
    {
        return 0;
    }

    template<class ID, class T>
    Position::Value Octree<ID, T>::LeafNode::LayerSize() const
    {
        return TileSize<Position::Value>;
    }

    template<class ID, class T>
    size_t Octree<ID, T>::LeafNode::TotalSize() const
    {
        return objects.size();
    }

    template<class ID, class T>
    void Octree<ID, T>::ParentNode::DeleteChildren()
    {
        for (auto& loop : children)
            loop.reset();
        totalSize = objects.size();
    }

    template<class ID, class T>
    Octree<ID, T>::ParentNode::ParentNode(
        LayerNumber layer,
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
        layer(layer),
        layerSize(layerSize)
    {}

    template<class ID, class T>
    Octree<ID, T>::ParentNode::ParentNode(ParentNode&& arg) noexcept :
        Node(std::move(arg)), children(std::move(arg.children)), bounds(std::move(arg.bounds)),
        totalSize(std::move(arg.totalSize)), layer(arg.layer), layerSize(arg.layerSize)
    {}

    template<class ID, class T>
    auto Octree<ID, T>::ParentNode::operator=(ParentNode&& arg) noexcept -> ParentNode&
    {
        Node::operator=(std::move(arg));
        children = std::move(arg.children);
        bounds = std::move(arg.bounds);
        totalSize = std::move(arg.totalSize);
        layer = arg.layer;
        return *this;
    }

    template<class ID, class T>
    bool Octree<ID, T>::ParentNode::Emplace(ID id, const T& emplace, const AxisAlignedBox3D& bounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!this->bounds.Contains(bounds))
            return false;

        // Try to emplace into the children
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            if (loop->Emplace(id, emplace, bounds))
            {
                ++totalSize;
                return true;
            }
        }

        // Wasn't emplaced into any children, but could require a new child

        auto determination = DetermineChild(layerSize, this->bounds, bounds);
        if (!determination)
        {
            objects.emplace_back(id, emplace);
            ++totalSize;
            return true;
        }

        NodePtr& selectedNode = children[determination->selection];
        assert(!selectedNode);
        selectedNode = CreateNode(
            layer - 1,
            FabricateLayerSize(layer - 1),
            (x * 2) + determination->xOffset,
            (y * 2) + determination->yOffset,
            (z * 2) + determination->zOffset);
        const bool emplaced = selectedNode->Emplace(id, emplace, bounds);
        if (emplaced)
            ++totalSize;

        return emplaced;
    }

    template<class ID, class T>
    bool Octree<ID, T>::ParentNode::Remove(ID id, const AxisAlignedBox3D& bounds)
    {
        // If this node is totally empty, then just leave
        if (IsTotalEmpty())
            return false;

        // Check if the fragment can fit entirely within this node
        if (!bounds.Contains(bounds))
            return false;

        // Check all children to see if they can remove it
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            if (loop->Remove(id, bounds))
            {
                // Delete the child node if it's empty
                if (loop->IsTotalEmpty())
                    loop.reset();

                --totalSize;
                return true;
            }
        }

        for(auto loop = objects.begin(); loop != objects.end(); ++loop)
        {
            if (loop->id == id)
            {
                objects.erase(loop);
                --totalSize;
                return true;
            }
        }

        return false;
    }

    template<class ID, class T>
    bool Octree<ID, T>::ParentNode::AddChild(Node& node)
    {
        auto determination = DetermineChild(layerSize, bounds, node.Bounds());
        if (!determination)
            return false;

        children[determination->selection].reset(&node);
        totalSize += node.TotalSize();
        return true;
    }

    template<class ID, class T>
    void Octree<ID, T>::ParentNode::Clear()
    {
        DeleteChildren();
        objects.clear();
        totalSize = 0;
    }

    template<class ID, class T>
    void Octree<ID, T>::ParentNode::AllInside(const AxisAlignedBox3D& aabb, List& list)
    {
        if (!aabb.Intersects(bounds))
            return;

        // Insert from children
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            loop->AllInside(aabb, list);
        }

        list.insert(list.begin(), objects.begin(), objects.end());
    }

    template<class ID, class T>
    bool Octree<ID, T>::ParentNode::IsTotalEmpty() const
    {
        return totalSize == 0;
    }

    template<class ID, class T>
    AxisAlignedBox3D Octree<ID, T>::ParentNode::Bounds() const
    {
        return bounds;
    }

    template<class ID, class T>
    auto Octree<ID, T>::ParentNode::Layer() const -> LayerNumber
    {
        return layer;
    }

    template<class ID, class T>
    Position::Value Octree<ID, T>::ParentNode::LayerSize() const
    {
        return layerSize;
    }

    template<class ID, class T>
    size_t Octree<ID, T>::ParentNode::TotalSize() const
    {
        return totalSize;
    }

    template<class ID, class T>
    Position::Value Octree<ID, T>::ParentNode::FabricateLayerSize(LayerNumber layer)
    {
        return TileSize<Position::Value> * PowerOfTwo(layer);
    }
}