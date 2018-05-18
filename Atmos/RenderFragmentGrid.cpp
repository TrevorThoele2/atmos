
#include "RenderFragmentGrid.h"
#include "RenderFragment.h"

#include "Assert.h"

namespace Atmos
{
    RenderFragmentGrid::ChildDetermination::ChildDetermination(ChildT selection, GridPosition::ValueT xOffset, GridPosition::ValueT yOffset, GridPosition::ValueT zOffset) : selection(selection), xOffset(xOffset), yOffset(yOffset), zOffset(zOffset)
    {}

    Optional<RenderFragmentGrid::ChildDetermination> RenderFragmentGrid::DetermineChild(GridPosition::ValueT layerSize, const AxisBoundingBox3D &bounds, const AxisBoundingBox3D &againstBounds)
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

    RenderFragmentGrid::NodeBase::NodeBase(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z) : x(x), y(y), z(z)
    {}

    RenderFragmentGrid::NodeBase::NodeBase(NodeBase &&arg) : x(std::move(arg.x)), y(std::move(arg.y)), z(std::move(arg.z))
    {}

    RenderFragmentGrid::NodeBase& RenderFragmentGrid::NodeBase::operator=(NodeBase &&arg)
    {
        const_cast<GridPosition::ValueT&>(x) = std::move(arg.x);
        const_cast<GridPosition::ValueT&>(y) = std::move(arg.y);
        const_cast<GridPosition::ValueT&>(z) = std::move(arg.z);
        return *this;
    }

    GridPosition RenderFragmentGrid::NodeBase::GetPosition() const
    {
        return GridPosition(x, y, z);
    }

    RenderFragmentGrid::LeafNode::LeafNode(GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z) : NodeBase(x, y, z)
    {}

    RenderFragmentGrid::LeafNode::LeafNode(LeafNode &&arg) : NodeBase(std::move(arg))
    {}

    RenderFragmentGrid::LeafNode& RenderFragmentGrid::LeafNode::operator=(LeafNode &&arg)
    {
        NodeBase::operator=(std::move(arg));
        return *this;
    }

    bool RenderFragmentGrid::LeafNode::Emplace(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Within(GetBounds()))
            return false;

        fragments.emplace(&fragment);
        return true;
    }

    bool RenderFragmentGrid::LeafNode::Remove(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds)
    {
        return fragments.erase(&fragment) != 0;
    }

    void RenderFragmentGrid::LeafNode::Clear()
    {
        fragments.clear();
    }

    void RenderFragmentGrid::LeafNode::Find(const AxisBoundingBox3D &aabb, RenderFragmentGrid::SetT &set)
    {
        // Check if the aabb overlaps this node
        if (!aabb.Overlapping(GetBounds()))
            return;

        set.insert(fragments.begin(), fragments.end());
    }

    bool RenderFragmentGrid::LeafNode::IsTotalEmpty() const
    {
        return fragments.empty();
    }

    AxisBoundingBox3D RenderFragmentGrid::LeafNode::GetBounds() const
    {
        GridPosition::ValueT gridSize(GRID_SIZE<GridPosition::ValueT>);
        return AxisBoundingBox3D(static_cast<AxisBoundingBox3D::Coordinate>(x * gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(y * gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(z * gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(x * gridSize + gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(y * gridSize + gridSize),
            static_cast<AxisBoundingBox3D::Coordinate>(z * gridSize + gridSize));
    }

    RenderFragmentGrid::LayerT RenderFragmentGrid::LeafNode::GetLayer() const
    {
        return 0;
    }

    GridPosition::ValueT RenderFragmentGrid::LeafNode::GetLayerSize() const
    {
        return GRID_SIZE<GridPosition::ValueT>;
    }

    RenderFragmentGrid::SetT::size_type RenderFragmentGrid::LeafNode::GetTotalSize() const
    {
        return fragments.size();
    }

    void RenderFragmentGrid::Node::DeleteChildren()
    {
        for (auto &loop : children)
            loop.reset();
        totalSize = fragments.size();
    }

    RenderFragmentGrid::Node::Node(LayerT layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z) : NodeBase(x, y, z), layer(layer), layerSize(layerSize), totalSize(0),
        bounds(static_cast<AxisBoundingBox3D::Coordinate>(x * layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>(y * layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>(z * layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>((x * layerSize) + layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>((y * layerSize) + layerSize),
            static_cast<AxisBoundingBox3D::Coordinate>((z * layerSize) + layerSize))
    {}

    RenderFragmentGrid::Node::Node(Node &&arg) : NodeBase(std::move(arg)), children(std::move(arg.children)), bounds(std::move(arg.bounds)), totalSize(std::move(arg.totalSize)), layer(arg.layer)
    {}

    RenderFragmentGrid::Node& RenderFragmentGrid::Node::operator=(Node &&arg)
    {
        NodeBase::operator=(std::move(arg));
        children = std::move(arg.children);
        bounds = std::move(arg.bounds);
        totalSize = std::move(arg.totalSize);
        layer = arg.layer;
        return *this;
    }

    bool RenderFragmentGrid::Node::Emplace(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds)
    {
        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Within(bounds))
            return false;

        // Try to emplace into the children
        for (auto &loop : children)
        {
            if (!loop)
                continue;

            if (loop->Emplace(fragment, fragmentBounds))
            {
                ++totalSize;
                return true;
            }
        }

        // Wasn't emplaced into any children, but could require a new child
        Optional<ChildDetermination> determination = DetermineChild(layerSize, bounds, fragmentBounds);
        if (!determination.IsValid())
        {
            fragments.emplace(&fragment);
            ++totalSize;
            return true;
        }

        NodePtr &selectedNode = children[determination->selection];
        ATMOS_ASSERT_MESSAGE(!selectedNode, "This should never be occupied.");
        selectedNode.reset(FabricateNode(layer - 1, FabricateLayerSize(layer - 1), (x * 2) + determination->xOffset, (y * 2) + determination->yOffset, (z * 2) + determination->zOffset));
        bool emplaced = selectedNode->Emplace(fragment, fragmentBounds);
        if (emplaced)
            ++totalSize;

        return emplaced;
    }

    bool RenderFragmentGrid::Node::Remove(RenderFragment &fragment, const AxisBoundingBox3D &fragmentBounds)
    {
        // If this node is totally empty, then just leave
        if (IsTotalEmpty())
            return false;

        // Check if the fragment can fit entirely within this node
        if (!fragmentBounds.Within(bounds))
            return false;

        // Check all children to see if they can remove it
        for (auto &loop : children)
        {
            if (!loop)
                continue;

            if (loop->Remove(fragment, fragmentBounds))
            {
                // Delete the child node if it's empty
                if (loop->IsTotalEmpty())
                    loop.reset();

                --totalSize;
                return true;
            }
        }

        bool erased = fragments.erase(&fragment) != 0;
        if (erased)
            --totalSize;
        return erased;
    }

    bool RenderFragmentGrid::Node::AddChild(NodeBase *node)
    {
        Optional<ChildDetermination> determination(DetermineChild(layerSize, bounds, node->GetBounds()));
        if (!determination)
            return false;

        children[determination->selection].reset(node);
        totalSize += node->GetTotalSize();
        return true;
    }

    void RenderFragmentGrid::Node::Clear()
    {
        DeleteChildren();
        fragments.clear();
        totalSize = 0;
    }

    void RenderFragmentGrid::Node::Find(const AxisBoundingBox3D &aabb, RenderFragmentGrid::SetT &set)
    {
        // Check if the aabb overlaps this node
        if (!aabb.Overlapping(bounds))
            return;

        // Insert from children
        for (auto &loop : children)
        {
            if (!loop)
                continue;

            loop->Find(aabb, set);
        }

        // Insert fragments
        set.insert(fragments.begin(), fragments.end());
    }

    bool RenderFragmentGrid::Node::IsTotalEmpty() const
    {
        return totalSize == 0;
    }

    AxisBoundingBox3D RenderFragmentGrid::Node::GetBounds() const
    {
        return bounds;
    }

    RenderFragmentGrid::LayerT RenderFragmentGrid::Node::GetLayer() const
    {
        return layer;
    }

    GridPosition::ValueT RenderFragmentGrid::Node::GetLayerSize() const
    {
        return layerSize;
    }

    RenderFragmentGrid::SetT::size_type RenderFragmentGrid::Node::GetTotalSize() const
    {
        return totalSize;
    }

    GridPosition::ValueT RenderFragmentGrid::Node::FabricateLayerSize(LayerT layer)
    {
        return GRID_SIZE<GridPosition::ValueT> * PowerOfTwo(layer);
    }

    RenderFragmentGrid::NodeBase* RenderFragmentGrid::FabricateNode(LayerT layer, GridPosition::ValueT layerSize, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z)
    {
        if (layer == 0)
            return new LeafNode(x, y, z);
        else
            return new Node(layer, layerSize, x, y, z);
    }

    void RenderFragmentGrid::FabricateHeads(LayerT layer)
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
            auto headCreator = [&](NodePtr &ptr, GridPosition::ValueT x, GridPosition::ValueT y, GridPosition::ValueT z)
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

    void RenderFragmentGrid::AddImpl(RenderFragment &add)
    {
        auto attemptEmplace = [&]() -> bool
        {
            for (auto &loop : heads)
            {
                if (loop->Emplace(add, add.GetBounds()))
                {
                    ++size;
                    return true;
                }
            }

            // Check if inside total
            if (IsWithinTotal(add.GetBounds()))
            {
                fragmentsInTotal.emplace(&add);
                ++size;
                return true;
            }

            return false;
        };

        while (!attemptEmplace())
            FabricateHeads(heads[0]->GetLayer() + 1);
    }

    void RenderFragmentGrid::RemoveImpl(RenderFragment &remove, const AxisBoundingBox3D &bounds)
    {
        fragmentsToAdd.erase(&remove);
        if (IsEmpty())
            return;

        for (auto &loop : heads)
        {
            if (loop->Remove(remove, bounds))
            {
                --size;
                return;
            }
        }

        if (IsWithinTotal(bounds) && fragmentsInTotal.erase(&remove) != 0)
            --size;
    }

    RenderFragmentGrid::RenderFragmentGrid() : size(0)
    {
        FabricateHeads(0);
    }

    RenderFragmentGrid::RenderFragmentGrid(RenderFragmentGrid &&arg) : size(std::move(arg.size)), heads(std::move(arg.heads)), fragmentsInTotal(std::move(arg.fragmentsInTotal)), fragmentsToAdd(std::move(arg.fragmentsToAdd))
    {}

    RenderFragmentGrid& RenderFragmentGrid::operator=(RenderFragmentGrid &&arg)
    {
        size = std::move(arg.size);
        heads = std::move(arg.heads);
        fragmentsInTotal = std::move(arg.fragmentsInTotal);
        fragmentsToAdd = std::move(arg.fragmentsToAdd);
        return *this;
    }

    void RenderFragmentGrid::Work()
    {
        if (fragmentsToAdd.empty())
            return;

        for (auto &loop : fragmentsToAdd)
            AddImpl(*loop);

        fragmentsToAdd.clear();
    }

    void RenderFragmentGrid::Add(RenderFragment &add)
    {
        if (fragmentsToAdd.find(&add) != fragmentsToAdd.end())
            return;

        RemoveImpl(add, add.GetBounds());
        fragmentsToAdd.emplace(&add);
    }

    void RenderFragmentGrid::Remove(RenderFragment &remove)
    {
        RemoveImpl(remove, remove.GetBounds());
    }

    void RenderFragmentGrid::Clear()
    {
        FabricateHeads(0);
        size = 0;
        fragmentsInTotal.clear();
        fragmentsToAdd.clear();
    }

    void RenderFragmentGrid::InformChanged(RenderFragment &changed, const AxisBoundingBox3D &previousBounds)
    {
        if (fragmentsToAdd.find(&changed) != fragmentsToAdd.end())
            return;

        RemoveImpl(changed, previousBounds);
        fragmentsToAdd.emplace(&changed);
    }

    RenderFragmentGrid::SetT RenderFragmentGrid::Find(const AxisBoundingBox3D &aabb)
    {
        SetT ret;
        for (auto &loop : heads)
            loop->Find(aabb, ret);

        if (GetTotalBounds().Overlapping(aabb))
            ret.insert(fragmentsInTotal.begin(), fragmentsInTotal.end());

        return ret;
    }

    bool RenderFragmentGrid::IsEmpty() const
    {
        return Size() == 0;
    }

    RenderFragmentGrid::size_type RenderFragmentGrid::Size() const
    {
        return size;
    }
}