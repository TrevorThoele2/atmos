
#include "RenderFragment.h"
#include "RenderFragmentHandler.h"
#include "CurrentField.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(RenderFragment)
    {
        ::Inscription::BaseSerialize<Sense>(scribe, *this);
        scribe(size);
        if (scribe.IsInput())
            CalculateBounds();
    }

    void RenderFragment::CalculateBounds()
    {
        AxisBoundingBox3D previousBounds(bounds);
        bounds.SetLeft(GetPosition().GetX() - (size.GetScaledWidth() / 2));
        bounds.SetTop(GetPosition().GetY() - (size.GetScaledHeight() / 2));
        bounds.SetFarZ(GetPosition().GetZ() - (size.GetScaledDepth() / 2));
        bounds.SetRight(bounds.GetLeft() + size.GetScaledWidth());
        bounds.SetBottom(bounds.GetTop() + size.GetScaledHeight());
        bounds.SetNearZ(bounds.GetFarZ() + size.GetScaledDepth());
        if(GetCurrentRenderFragments())
            GetCurrentRenderFragments()->FragmentChanged(*this, previousBounds);
    }

    void RenderFragment::OnPositionChanged()
    {
        CalculateBounds();
    }

    void RenderFragment::OnEnabledChanged()
    {

    }

    RenderFragment::RenderFragment(bool enabled) : Sense(enabled)
    {}

    void RenderFragment::SetWidth(Size3D::ValueT set)
    {
        if (!IsEnabled())
            return;

        size.SetWidth(set);
        CalculateBounds();
    }

    void RenderFragment::SetHeight(Size3D::ValueT set)
    {
        if (!IsEnabled())
            return;

        size.SetHeight(set);
        CalculateBounds();
    }

    void RenderFragment::SetDepth(Size3D::ValueT set)
    {
        if (!IsEnabled())
            return;

        size.SetDepth(set);
        CalculateBounds();
    }

    void RenderFragment::SetScalers(const Join3<Size3D::ValueT> &set)
    {
        if (!IsEnabled())
            return;

        size.SetXScaler(set.first);
        size.SetYScaler(set.second);
        size.SetZScaler(set.third);
        CalculateBounds();
    }

    void RenderFragment::SetXScaler(Size3D::ValueT set)
    {
        if (!IsEnabled())
            return;

        size.SetXScaler(set);
        CalculateBounds();
    }

    void RenderFragment::SetYScaler(Size3D::ValueT set)
    {
        if (!IsEnabled())
            return;

        size.SetYScaler(set);
        CalculateBounds();
    }

    void RenderFragment::SetZScaler(Size3D::ValueT set)
    {
        if (!IsEnabled())
            return;

        size.SetZScaler(set);
        CalculateBounds();
    }

    RenderFragment::RenderFragment(const RenderFragment &arg) : Sense(arg), size(arg.size), bounds(arg.bounds)
    {}

    RenderFragment::RenderFragment(RenderFragment &&arg) : Sense(arg), size(std::move(arg.size)), bounds(std::move(arg.bounds)), handler(std::move(arg.handler))
    {}

    RenderFragment& RenderFragment::operator=(const RenderFragment &arg)
    {
        Sense::operator=(arg);
        size = arg.size;
        bounds = arg.bounds;
        handler = arg.handler;
        return *this;
    }

    RenderFragment& RenderFragment::operator=(RenderFragment &&arg)
    {
        Sense::operator=(std::move(arg));
        size = std::move(arg.size);
        bounds = std::move(arg.bounds);
        handler = std::move(arg.handler);
        return *this;
    }

    RenderFragment::~RenderFragment()
    {
        if (handler)
            handler->Remove(*this);
    }

    bool RenderFragment::operator==(const RenderFragment &arg) const
    {
        return Sense::operator==(arg) && size == arg.size && bounds == arg.bounds && handler == arg.handler;
    }

    bool RenderFragment::operator!=(const RenderFragment &arg) const
    {
        return !(*this == arg);
    }

    void RenderFragment::Draw()
    {
        if (!IsEnabled())
            return;

        DrawImpl();
    }

    const Size3D& RenderFragment::GetSize() const
    {
        return size;
    }

    Size3D::ValueT RenderFragment::GetWidth() const
    {
        return size.GetWidth();
    }

    Size3D::ValueT RenderFragment::GetHeight() const
    {
        return size.GetHeight();
    }

    Size3D::ValueT RenderFragment::GetDepth() const
    {
        return size.GetDepth();
    }

    Join3<Size3D::ValueT> RenderFragment::GetScalers() const
    {
        return size.GetScalers();
    }

    Size3D::ValueT RenderFragment::GetXScaler() const
    {
        return size.GetXScaler();
    }

    Size3D::ValueT RenderFragment::GetYScaler() const
    {
        return size.GetYScaler();
    }

    Size3D::ValueT RenderFragment::GetZScaler() const
    {
        return size.GetZScaler();
    }

    void RenderFragment::SetXRotation(const Angle &set)
    {
        if (!IsEnabled())
            return;

        size.SetXRotation(set);
        CalculateBounds();
    }

    void RenderFragment::SetYRotation(const Angle &set)
    {
        if (!IsEnabled())
            return;

        size.SetYRotation(set);
        CalculateBounds();
    }

    void RenderFragment::SetZRotation(const Angle &set)
    {
        if (!IsEnabled())
            return;

        size.SetZRotation(set);
        CalculateBounds();
    }

    const Angle& RenderFragment::GetXRotation() const
    {
        return size.GetXRotation();
    }

    const Angle& RenderFragment::GetYRotation() const
    {
        return size.GetYRotation();
    }

    const Angle& RenderFragment::GetZRotation() const
    {
        return size.GetZRotation();
    }

    const AxisBoundingBox3D& RenderFragment::GetBounds() const
    {
        return bounds;
    }
}