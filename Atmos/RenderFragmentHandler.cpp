
#include "RenderFragmentHandler.h"
#include "Camera.h"
#include "AvatarSystem.h"

namespace Atmos
{
    void RenderFragmentHandler::OnCameraMoved()
    {
        //Cull();
    }

    void RenderFragmentHandler::OnAfterFieldSet()
    {
        //Cull();
    }

    void RenderFragmentHandler::FragmentChanged(Fragment &fragment, const AxisBoundingBox3D &previousBounds)
    {
        if (totalFragments.find(&fragment) == totalFragments.end())
            return;

        grid.InformChanged(fragment, previousBounds);
    }

    void RenderFragmentHandler::Emplace(Fragment &fragment)
    {
        if (!totalFragments.emplace(&fragment).second)
            return;

        dirty = true;
        grid.Add(fragment);
        fragment.handler = this;
    }

    RenderFragmentHandler::RenderFragmentHandler(Field &field) : FieldComponent(field), dirty(false)
    {
        eventSlots.Add(Camera::Instance().eventMoved.Subscribe(&RenderFragmentHandler::OnCameraMoved, *this));
    }

    RenderFragmentHandler::RenderFragmentHandler(Field &field, RenderFragmentHandler &&arg) : FieldComponent(field), dirty(std::move(arg.dirty)), totalFragments(std::move(arg.totalFragments)), grid(std::move(arg.grid)), culled(std::move(arg.culled)), zStopOverride(std::move(arg.zStopOverride))
    {
        eventSlots.Add(Camera::Instance().eventMoved.Subscribe(&RenderFragmentHandler::OnCameraMoved, *this));
    }

    RenderFragmentHandler::~RenderFragmentHandler()
    {
        for (auto &fragLoop : totalFragments)
            fragLoop->handler = nullptr;
    }

    RenderFragmentHandler& RenderFragmentHandler::operator=(RenderFragmentHandler &&arg)
    {
        dirty = std::move(arg.dirty);
        totalFragments = std::move(arg.totalFragments);
        grid = std::move(arg.grid);
        culled = std::move(arg.culled);
        zStopOverride = std::move(arg.zStopOverride);
        return *this;
    }

    void RenderFragmentHandler::Add(Fragment &fragment)
    {
        Emplace(fragment);
    }

    void RenderFragmentHandler::Remove(Fragment &remove)
    {
        if (totalFragments.erase(&remove) == 0)
            return;

        dirty = true;
        grid.Remove(remove);
        remove.handler = nullptr;
    }

    bool RenderFragmentHandler::Has(const Fragment &fragment) const
    {
        return totalFragments.find(const_cast<Fragment*>(&fragment)) != totalFragments.end();
    }

    void RenderFragmentHandler::Render()
    {
        for (auto &fragLoop : culled)
            fragLoop->Draw();
    }

    void RenderFragmentHandler::Cull()
    {
        auto &sides = Camera::GetSides();
        RenderFragmentGrid::SetT set;
        if(!zStopOverride.IsValid())
            set = std::move(grid.Find(AxisBoundingBox3D(sides.GetLeft(), sides.GetTop(), std::numeric_limits<AxisBoundingBox3D::Coordinate>::lowest(), sides.GetRight(), sides.GetBottom(), std::numeric_limits<AxisBoundingBox3D::Coordinate>::max())));
        else
            set = std::move(grid.Find(AxisBoundingBox3D(sides.GetLeft(), sides.GetTop(), std::numeric_limits<AxisBoundingBox3D::Coordinate>::lowest(), sides.GetRight(), sides.GetBottom(), zStopOverride.Get())));
    
        culled.clear();
        for (auto &fragLoop : set)
            culled.emplace(fragLoop);
    }

    void RenderFragmentHandler::Work()
    {
        grid.Work();
        if (!dirty)
            return;

        Cull();
        dirty = false;
    }

    void RenderFragmentHandler::Clear()
    {
        if(!grid.IsEmpty())
            dirty = true;
        grid.Clear();
    }

    void RenderFragmentHandler::SetZOverride(Position3D::ValueT set)
    {
        if (zStopOverride == set)
            return;

        dirty = true;
        zStopOverride.Set(set);
    }

    void RenderFragmentHandler::SetZOverrideToMax()
    {
        if (zStopOverride == std::numeric_limits<Position3D::ValueT>::max())
            return;

        dirty = true;
        zStopOverride.Set(std::numeric_limits<Position3D::ValueT>::max());
    }

    void RenderFragmentHandler::ResetZOverride()
    {
        if (!zStopOverride.IsValid())
            return;

        dirty = true;
        zStopOverride.Reset();
    }

    const Optional<Position3D::ValueT>& RenderFragmentHandler::GetZOverride() const
    {
        return zStopOverride;
    }

    Position3D::ValueT RenderFragmentHandler::GetZ() const
    {
        if (zStopOverride.IsValid())
            return zStopOverride.Get();
        else if (::Atmos::Ent::AvatarSystem::IsValid())
            return static_cast<Position3D::ValueT>(::Atmos::Ent::AvatarSystem::GetPosition().z + 1);
        else
            return std::numeric_limits<Position3D::ValueT>::max();
    }
}