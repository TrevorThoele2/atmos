#pragma once

#include "Size3D.h"
#include "AxisAlignedBox3D.h"
#include "GridCellSize.h"

#include "MathUtility.h"
#include "SpatialAlgorithms.h"

namespace Atmos::Spatial::Grid
{
    template<class ID, class Value>
    class Octree
    {
    public:
        using Bounds = AxisAlignedBox3D;
    public:
        struct Object
        {
            ID id;
            Bounds bounds;
            Value value;

            Object(ID id, Bounds bounds, const Value& value) : id(id), bounds(bounds), value(value)
            {}

            Object(ID id, Bounds bounds, Value&& value) : id(id), bounds(bounds), value(std::move(value))
            {}
        };
    public:
        explicit Octree(Point3D center = Point3D());

        void Add(ID id, const Value& value, Bounds bounds);
        void Add(ID id, Value&& value, Bounds bounds);
        bool Remove(ID id, Bounds bounds);
        void Move(ID id, Value value, Bounds oldBounds, Bounds newBounds);

        [[nodiscard]] std::vector<const Object*> AllWithin(Bounds bounds) const;

        [[nodiscard]] Bounds TotalBounds() const;
        [[nodiscard]] size_t Size() const;
    private:
        struct ChosenChild
        {
            bool left = false;
            bool top = false;
            bool farZ = false;

            ChosenChild() = default;
            ChosenChild(bool left, bool top, bool farZ) : left(left), top(top), farZ(farZ)
            {}
        };

        enum ChildIndex : size_t
        {
            LeftTopFar = 0,
            LeftTopNear,
            LeftBottomFar,
            LeftBottomNear,
            RightTopFar,
            RightTopNear,
            RightBottomFar,
            RightBottomNear,
        };

        class Node;
        using NodePtr = std::unique_ptr<Node>;
        using Nodes = std::array<NodePtr, 8>;

        using Scale = int;

        class Node
        {
        public:
            using Objects = std::vector<Object>;
            Objects objects;

            Nodes children;

            size_t totalSize = 0;

            typename Objects::iterator FindObject(ID id);

            void IntersectingChildren(
                Bounds bounds,
                std::vector<const Node*>& nodes,
                Point3D parentCenter,
                Scale parentScale) const;

            void Subdivide();
            void Unify();
            [[nodiscard]] bool IsSubdivided() const;

            [[nodiscard]] bool ChildrenAreEmpty() const;
        };

        Node head;
        Scale headScale = 1;
        Point3D headCenter;

        template<class T>
        void AddCommon(ID id, T value, Bounds bounds);

        [[nodiscard]] std::vector<Node*> NodeHierarchy(Bounds bounds);
        [[nodiscard]] std::vector<Node*> NodeHierarchySubdivide(Bounds bounds);

        [[nodiscard]] static std::optional<ChosenChild> ChooseChild(Point3D center, Bounds bounds);
        [[nodiscard]] static size_t IndexFor(ChosenChild chosenChild);
        [[nodiscard]] static Point3D CenterForChild(Point3D nodeCenter, ChosenChild chosenChild, Scale scale);
        [[nodiscard]] static Bounds BoundsFor(Point3D center, Scale scale);
        [[nodiscard]] static bool CanBeSubdivided(Scale scale);
        [[nodiscard]] static Point3D::Value LengthFor(Scale scale);
    };

    template <class ID, class Value>
    Octree<ID, Value>::Octree(Point3D center) : headCenter(center)
    {}

    template<class ID, class Value>
    void Octree<ID, Value>::Add(ID id, const Value& value, Bounds bounds)
    {
        AddCommon(id, value, bounds);
    }

    template<class ID, class Value>
    void Octree<ID, Value>::Add(ID id, Value&& value, Bounds bounds)
    {
        AddCommon(id, std::move(value), bounds);
    }

    template<class ID, class Value>
    bool Octree<ID, Value>::Remove(ID id, Bounds bounds)
    {
        const auto hierarchy = NodeHierarchy(bounds);
        const auto lastNode = hierarchy[hierarchy.size() - 1];
        const auto foundObject = lastNode->FindObject(id);
        if (foundObject == lastNode->objects.end())
            return false;

        lastNode->objects.erase(foundObject);
        for (auto node = hierarchy.rbegin(); node != hierarchy.rend(); ++node)
        {
            --(*node)->totalSize;

            if (!(*node)->IsSubdivided())
                continue;

            if ((*node)->ChildrenAreEmpty())
                (*node)->children = Nodes();
        }

        if (head.ChildrenAreEmpty() && head.objects.empty())
            headScale = 1;

        return true;
    }

    template<class ID, class Value>
    void Octree<ID, Value>::Move(ID id, Value value, Bounds oldBounds, Bounds newBounds)
    {
        if (!Remove(id, oldBounds))
            return;
        Add(id, value, newBounds);
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::AllWithin(Bounds bounds) const -> std::vector<const Object*>
    {
        std::vector<const Object*> objects;

        if (!Intersects(BoundsFor(headCenter, headScale), bounds))
            return objects;

        std::vector<const Node*> nodes;
        nodes.push_back(&head);
        head.IntersectingChildren(bounds, nodes, headCenter, headScale);
        for (auto& node : nodes)
            for (auto& object : node->objects)
                if (Intersects(object.bounds, bounds))
                    objects.push_back(&object);

        return objects;
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::TotalBounds() const -> Bounds
    {
        return BoundsFor(headCenter, headScale);
    }

    template<class ID, class Value>
    size_t Octree<ID, Value>::Size() const
    {
        return head.totalSize;
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::Node::FindObject(ID id) -> typename Objects::iterator
    {
        return std::find_if(
            objects.begin(),
            objects.end(),
            [id](const Object& object)
            {
                return object.id == id;
            });
    }

    template<class ID, class Value>
    void Octree<ID, Value>::Node::IntersectingChildren(
        Bounds bounds,
        std::vector<const Node*>& nodes,
        Point3D parentCenter,
        Scale parentScale) const
    {
        if (!IsSubdivided())
            return;

        const auto childScale = parentScale - 1;

        auto attemptAddToNodes = [&nodes, this, parentCenter, childScale, bounds](ChosenChild chosenChild)
        {
            const auto childCenter = CenterForChild(parentCenter, chosenChild, childScale);
            const auto childBounds = BoundsFor(childCenter, childScale);
            if (Intersects(childBounds, bounds))
            {
                const auto childIndex = IndexFor(chosenChild);
                auto child = children[childIndex].get();
                nodes.push_back(child);
                child->IntersectingChildren(bounds, nodes, childCenter, childScale);
            }
        };

        attemptAddToNodes(ChosenChild(true, true, true));
        attemptAddToNodes(ChosenChild(true, true, false));
        attemptAddToNodes(ChosenChild(true, false, true));
        attemptAddToNodes(ChosenChild(true, false, false));
        attemptAddToNodes(ChosenChild(false, true, true));
        attemptAddToNodes(ChosenChild(false, true, false));
        attemptAddToNodes(ChosenChild(false, false, true));
        attemptAddToNodes(ChosenChild(false, false, false));
    }

    template<class ID, class Value>
    void Octree<ID, Value>::Node::Subdivide()
    {
        if (IsSubdivided())
            return;

        for (auto& child : children)
            child = std::make_unique<Node>();
    }

    template<class ID, class Value>
    void Octree<ID, Value>::Node::Unify()
    {
        if (!IsSubdivided())
            return;

        children = Nodes();
    }

    template<class ID, class Value>
    bool Octree<ID, Value>::Node::IsSubdivided() const
    {
        return children[0] != nullptr;
    }

    template<class ID, class Value>
    bool Octree<ID, Value>::Node::ChildrenAreEmpty() const
    {
        if (!IsSubdivided())
            return true;

        for (auto& child : children)
            if (child->totalSize != 0)
                return false;

        return true;
    }

    template<class ID, class Value>
    template<class T>
    void Octree<ID, Value>::AddCommon(ID id, T value, Bounds bounds)
    {
        while (!Contains(BoundsFor(headCenter, headScale), bounds))
        {
            ++headScale;

            if (!head.ChildrenAreEmpty())
            {
                auto previousChildren = std::move(head.children);

                head.Subdivide();

                const auto childSetter = [this, &previousChildren](
                    size_t headChildIndex,
                    size_t setChildIndex,
                    size_t previousChildIndex)
                {
                    auto& headChild = head.children[headChildIndex];
                    headChild->Subdivide();
                    headChild->children[setChildIndex] = std::move(previousChildren[previousChildIndex]);
                    headChild->totalSize = headChild->children[setChildIndex]->totalSize;
                };

                childSetter(0, 7, 0);
                childSetter(1, 6, 1);
                childSetter(2, 5, 2);
                childSetter(3, 4, 3);
                childSetter(4, 3, 4);
                childSetter(5, 2, 5);
                childSetter(6, 1, 6);
                childSetter(7, 0, 7);
            }
            else
                head.Unify();
        }

        const auto hierarchy = NodeHierarchySubdivide(bounds);
        for(auto& node : hierarchy)
            ++node->totalSize;

        hierarchy[hierarchy.size() - 1]->objects.emplace_back(id, bounds, std::forward<T>(value));
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::NodeHierarchy(Bounds bounds) -> std::vector<Node*>
    {
        std::vector<Node*> nodeHierarchy;

        auto focusedNode = &head;
        auto focusedCenter = headCenter;
        auto focusedScale = headScale;
        while (true)
        {
            nodeHierarchy.push_back(focusedNode);

            if (!focusedNode->IsSubdivided())
                break;
             
            const auto chosenChild = ChooseChild(focusedCenter, bounds);
            if (!chosenChild)
                break;

            const auto childCenter = CenterForChild(focusedCenter, *chosenChild, focusedScale);
            const auto childScale = focusedScale - 1;
            if (Contains(BoundsFor(childCenter, childScale), bounds))
            {
                const auto childIndex = IndexFor(*chosenChild);
                focusedNode = focusedNode->children[childIndex].get();
                focusedCenter = childCenter;
                focusedScale = childScale;
            }
            else
                break;
        }

        return nodeHierarchy;
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::NodeHierarchySubdivide(Bounds bounds) -> std::vector<Node*>
    {
        std::vector<Node*> nodeHierarchy;

        auto focusedNode = &head;
        auto focusedCenter = headCenter;
        auto focusedScale = headScale;
        while (true)
        {
            nodeHierarchy.push_back(focusedNode);

            const auto chosenChild = ChooseChild(focusedCenter, bounds);
            if (!chosenChild)
                break;

            const auto childCenter = CenterForChild(focusedCenter, *chosenChild, focusedScale);
            const auto childScale = focusedScale - 1;
            if (Contains(BoundsFor(childCenter, childScale), bounds))
            {
                if (!focusedNode->IsSubdivided())
                    focusedNode->Subdivide();

                const auto childIndex = IndexFor(*chosenChild);
                focusedNode = focusedNode->children[childIndex].get();
                focusedCenter = childCenter;
                focusedScale = childScale;
            }
            else
                break;
        }

        return nodeHierarchy;
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::ChooseChild(Point3D center, Bounds bounds) -> std::optional<ChosenChild>
    {
        ChosenChild chosenChild;

        if (bounds.Left() >= center.x)
            chosenChild.left = false;
        else if (bounds.Right() > center.x)
            return {};
        else
            chosenChild.left = true;

        if (bounds.Top() >= center.y)
            chosenChild.top = false;
        else if (bounds.Bottom() > center.y)
            return {};
        else
            chosenChild.top = true;

        if (bounds.FarZ() >= center.z)
            chosenChild.farZ = false;
        else if (bounds.NearZ() > center.z)
            return {};
        else
            chosenChild.farZ = true;

        return chosenChild;
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::IndexFor(ChosenChild chosenChild) -> size_t
    {
        size_t index = 0;
        if (!chosenChild.left)
            index += 4;

        if (!chosenChild.top)
            index += 2;

        if (!chosenChild.farZ)
            index += 1;

        return index;
    }

    template<class ID, class Value>
    Point3D Octree<ID, Value>::CenterForChild(Point3D nodeCenter, ChosenChild chosenChild, Scale scale)
    {
        const auto length = LengthFor(scale);
        const auto shiftLength = length / 2;

        return Point3D
        {
            chosenChild.left ? nodeCenter.x - shiftLength : nodeCenter.x + shiftLength,
            chosenChild.top ? nodeCenter.y - shiftLength : nodeCenter.y + shiftLength,
            chosenChild.farZ ? nodeCenter.z - shiftLength : nodeCenter.z + shiftLength
        };
    }

    template<class ID, class Value>
    auto Octree<ID, Value>::BoundsFor(Point3D center, Scale scale) -> Bounds
    {
        const auto length = LengthFor(scale);
        return Bounds
        {
            center,
            Size3D { length, length, length }
        };
    }

    template<class ID, class Value>
    bool Octree<ID, Value>::CanBeSubdivided(Scale scale)
    {
        return scale > 0;
    }

    template<class ID, class Value>
    Point3D::Value Octree<ID, Value>::LengthFor(Scale scale)
    {
        return CellSize<Size3D::Value> * PowerOfTwo(static_cast<Size3D::Value>(scale));
    }
}