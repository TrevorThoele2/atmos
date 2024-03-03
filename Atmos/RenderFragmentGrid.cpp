
#include "RenderFragmentGrid.h"
#include "RenderFragment.h"

#include "MathUtility.h"

#include "Assert.h"

namespace Atmos
{
    Octree::Octree() : size(0)
    {
        FabricateHeads(0);
    }

    Octree::Octree(Octree&& arg) :
        size(std::move(arg.size)), heads(std::move(arg.heads)), totalObjects(std::move(arg.totalObjects)), toAddObjects(std::move(arg.toAddObjects))
    {}

    Octree& Octree::operator=(Octree&& arg)
    {
        size = std::move(arg.size);
        heads = std::move(arg.heads);
        totalObjects = std::move(arg.totalObjects);
        toAddObjects = std::move(arg.toAddObjects);
        return *this;
    }

    void Octree::Work()
    {
        if (totalObjects.IsEmpty())
            return;

        for (auto& loop : toAddObjects)
            AddImpl(loop);

        toAddObjects.Clear();
    }

    void Octree::Add(ItemReference add)
    {
        if (toAddObjects.Has(add))
            return;

        RemoveImpl(add, add->bounds);
        toAddObjects.Add(add);
    }

    void Octree::Remove(ItemReference remove)
    {
        RemoveImpl(remove, remove->bounds);
    }

    void Octree::Clear()
    {
        FabricateHeads(0);
        size = 0;
        totalObjects.Clear();
        toAddObjects.Clear();
    }

    void Octree::InformChanged(ItemReference changed, const AxisBoundingBox3D& previousBounds)
    {
        if (toAddObjects.Has(changed))
            return;

        RemoveImpl(changed, previousBounds);
        toAddObjects.Add(changed);
    }

    Octree::Map Octree::AllInside(const AxisBoundingBox3D& aabb)
    {
        Map ret;
        for (auto& loop : heads)
            loop->AllInside(aabb, ret);

        if (GetTotalBounds().Overlapping(aabb))
            ret.Insert(totalObjects.begin(), totalObjects.end());

        return ret;
    }

    bool Octree::IsEmpty() const
    {
        return Size() == 0;
    }

    Octree::SizeT Octree::Size() const
    {
        return size;
    }

    Octree::NodeBase* Octree::FabricateNode(Layer layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z)
    {
        if (layer == 0)
            return new LeafNode(x, y, z);
        else
            return new Node(layer, layerSize, x, y, z);
    }

    void Octree::FabricateHeads(Layer layer)
    {
        if (layer == 0)
        {
            heads[WNF].reset(new LeafNode(-1, -1, -1));
            heads[ENF].reset(new LeafNode(0, -1, -1));
            heads[WSF].reset(new LeafNode(-1, 0, -1));
            heads[ESF].reset(new LeafNode(0, 0, -1));
            heads[WNN].reset(new LeafNode(-1, -1, 0));
            heads[ENN].reset(new LeafNode(0, -1, 0));
            heads[WSN].reset(new LeafNode(-1, 0, 0));
            heads[ESN].reset(new LeafNode(0, 0, 0));
        }
        else
        {
            auto headCreator = [&](NodePtr& ptr, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z)
            {
                auto prevHead = ptr.release();
                ptr.reset(new Node(layer, Node::FabricateLayerSize(layer), x, y, z));
                if (prevHead->IsTotalEmpty())
                    delete prevHead;
                else
                {
                    if (!static_cast<Node*>(ptr.get())->AddChild(prevHead))
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

    void Octree::AddImpl(ItemReference add)
    {
        auto attemptEmplace = [&]() -> bool
        {
            for (auto& loop : heads)
            {
                if (loop->Emplace(add, add->bounds))
                {
                    ++size;
                    return true;
                }
            }

            // Check if inside total
            if (IsWithinTotal(add->bounds))
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

    void Octree::RemoveImpl(ItemReference remove, const AxisBoundingBox3D& bounds)
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

    Octree::ChildDetermination::ChildDetermination(ChildID selection, GridPosition::ValueT xOffset, GridPosition::ValueT yOffset, GridPosition::ValueT zOffset) : selection(selection), xOffset(xOffset), yOffset(yOffset), zOffset(zOffset)
    {}

    Optional<Octree::ChildDetermination> Octree::DetermineChild(GridPosition::ValueT layerSize, const AxisBoundingBox3D& bounds, const AxisBoundingBox3D& againstBounds)
    {
        typedef Optional<ChildDetermination> RetT;

        const AxisBoundingBox3D::Coordinate midpointX = bounds.GetLeft() + (layerSize / 2.0f);
        const AxisBoundingBox3D::Coordinate midpointY = bounds.GetTop() + (layerSize / 2.0f);
        const AxisBoundingBox3D::Coordinate midpointZ = bounds.GetFarZ() + (layerSize / 2.0f);

        ChildDetermination determination(0, 0, 0, 0);

        // EAST = +0
        if (againstBounds.GetLeft() >= midpointX)
        {
            // WEST = +1
            determination.selection += 1;
            determination.xOffset = 1;
        }
        else if (againstBounds.GetRight() > midpointX)
        {
            // The right is > midpointX and left < midpointX, which means that this crosses between two grids - cannot be divided further
            // So, must be here
            return RetT();
        }

        // NORTH = +0
        if (againstBounds.GetTop() >= midpointY)
        {
            // SOUTH = +2
            determination.selection += 2;
            determination.yOffset = 1;
        }
        else if (againstBounds.GetBottom() > midpointY)
        {
            // The bottom is > midpointY and top < midpointY, which means that this crosses between two grids - cannot be divided further
            // So, must be here
            return RetT();
        }

        // FAR = +0
        if (againstBounds.GetFarZ() >= midpointZ)
        {
            // NEAR = +4
            determination.selection += 4;
            determination.zOffset = 1;
        }
        else if (againstBounds.GetNearZ() > midpointZ)
        {
            // The nearZ is > midpointZ and farZ < midpointZ, which means that this crosses between two grids - cannot be divided further
            // So, must be here
            return RetT();
        }

        return RetT(std::move(determination));
    }

    Octree::NodeBase::NodeBase(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z) : x(x), y(y), z(z)
    {}

    Octree::NodeBase::NodeBase(NodeBase&& arg) : x(std::move(arg.x)), y(std::move(arg.y)), z(std::move(arg.z))
    {}

    Octree::NodeBase& Octree::NodeBase::operator=(NodeBase&& arg)
    {
        const_cast<GridPosition::ValueT&>(x) = std::move(arg.x);
        const_cast<GridPosition::ValueT&>(y) = std::move(arg.y);
        const_cast<GridPosition::ValueT&>(z) = std::move(arg.z);
        return *this;
    }

    Octree::NodeBase::~NodeBase()
    {}

    GridPosition Octree::NodeBase::GetPosition() const
    {
        return GridPosition(x, y, z);
    }

    Octree::LeafNode::LeafNode(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z) : NodeBase(x, y, z)
    {}

    Octree::LeafNode::LeafNode(LeafNode&& arg) : NodeBase(std::move(arg))
    {}

    Octree::LeafNode& Octree::LeafNode::operator=(LeafNode&& arg)
    {
        NodeBase::operator=(std::move(arg));
        return *this;
    }

    bool Octree::LeafNode::Emplace(ObjectReference emplace, const AxisBoundingBox3D& fragmentBounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Within(GetBounds()))
            return false;

        objects.Add(emplace);
        return true;
    }

    bool Octree::LeafNode::Remove(ObjectReference remove, const AxisBoundingBox3D& fragmentBounds)
    {
        return objects.Remove(remove);
    }

    void Octree::LeafNode::Clear()
    {
        objects.Clear();
    }

    void Octree::LeafNode::AllInside(const AxisBoundingBox3D& aabb, Octree::Map& map)
    {
        // Check if the aabb overlaps this node
        if (!aabb.Overlapping(GetBounds()))
            return;

        map.Insert(objects.begin(), objects.end());
    }

    bool Octree::LeafNode::IsTotalEmpty() const
    {
        return objects.IsEmpty();
    }

    AxisBoundingBox3D Octree::LeafNode::GetBounds() const
    {
        GridPosition::ValueT gridSize(GRID_SIZE<GridPosition::ValueT>);
        return AxisBoundingBox3D(static_cast<AxisBoundingBox3D::Coordinate>(x * gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(y * gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(z * gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(x * gridSize + gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(y * gridSize + gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(z * gridSize + gridSize));
    }

    Octree::Layer Octree::LeafNode::GetLayer() const
    {
        return 0;
    }

    GridPosition::ValueT Octree::LeafNode::GetLayerSize() const
    {
        return GRID_SIZE<GridPosition::ValueT>;
    }

    Octree::Map::SizeT Octree::LeafNode::GetTotalSize() const
    {
        return objects.Size();
    }

    void Octree::Node::DeleteChildren()
    {
        for (auto& loop : children)
            loop.reset();
        totalSize = objects.Size();
    }

    Octree::Node::Node(Layer layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z) : NodeBase(x, y, z), layer(layer), layerSize(layerSize), totalSize(0),
        bounds(static_cast<AxisBoundingBox3D::Coordinate>(x * layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>(y * layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>(z * layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>((x * layerSize) + layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>((y * layerSize) + layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>((z * layerSize) + layerSize))
    {}

    Octree::Node::Node(Node&& arg) : NodeBase(std::move(arg)), children(std::move(arg.children)), bounds(std::move(arg.bounds)), totalSize(std::move(arg.totalSize)), layer(arg.layer)
    {}

    Octree::Node& Octree::Node::operator=(Node&& arg)
    {
        NodeBase::operator=(std::move(arg));
        children = std::move(arg.children);
        bounds = std::move(arg.bounds);
        totalSize = std::move(arg.totalSize);
        layer = arg.layer;
        return *this;
    }

    bool Octree::Node::Emplace(ObjectReference emplace, const AxisBoundingBox3D& fragmentBounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Within(bounds))
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
        Optional<ChildDetermination> determination = DetermineChild(layerSize, bounds, fragmentBounds);
        if (!determination.IsValid())
        {
            objects.Add(emplace);
            ++totalSize;
            return true;
        }

        NodePtr &selectedNode = children[determination->selection];
        ATMOS_ASSERT_MESSAGE(!selectedNode, "This should never be occupied.");
        selectedNode.reset(FabricateNode(layer - 1, FabricateLayerSize(layer - 1), (x * 2) + determination->xOffset, (y * 2) + determination->yOffset, (z * 2) + determination->zOffset));
        bool emplaced = selectedNode->Emplace(emplace, fragmentBounds);
        if (emplaced)
            ++totalSize;

        return emplaced;
    }

    bool Octree::Node::Remove(ObjectReference remove, const AxisBoundingBox3D& fragmentBounds)
    {
        // If this node is totally empty, then just leave
        if (IsTotalEmpty())
            return false;

        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Within(bounds))
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

        bool erased = objects.Remove(remove);
        if (erased)
            --totalSize;
        return erased;
    }

    bool Octree::Node::AddChild(NodeBase* node)
    {
        Optional<ChildDetermination> determination(DetermineChild(layerSize, bounds, node->GetBounds()));
        if (!determination)
            return false;

        children[determination->selection].reset(node);
        totalSize += node->GetTotalSize();
        return true;
    }

    void Octree::Node::Clear()
    {
        DeleteChildren();
        objects.Clear();
        totalSize = 0;
    }

    void Octree::Node::AllInside(const AxisBoundingBox3D& aabb, Octree::Map& map)
    {
        // Check if the aabb overlaps this node
        if (!aabb.Overlapping(bounds))
            return;

        // Insert from children
        for (auto& loop : children)
        {
            if (!loop)
                continue;

            loop->AllInside(aabb, map);
        }

        // Insert fragments
        map.Insert(objects.begin(), objects.end());
    }

    bool Octree::Node::IsTotalEmpty() const
    {
        return totalSize == 0;
    }

    AxisBoundingBox3D Octree::Node::GetBounds() const
    {
        return bounds;
    }

    Octree::Layer Octree::Node::GetLayer() const
    {
        return layer;
    }

    GridPosition::ValueT Octree::Node::GetLayerSize() const
    {
        return layerSize;
    }

    Octree::Map::SizeT Octree::Node::GetTotalSize() const
    {
        return totalSize;
    }

    GridPosition::ValueT Octree::Node::FabricateLayerSize(Layer layer)
    {
        return GRID_SIZE<GridPosition::ValueT> * PowerOfTwo(layer);
    }
}