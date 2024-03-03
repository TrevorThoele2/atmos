#pragma once

#include "Sense.h"

#include "Size3D.h"
#include "AxisBoundingBox3D.h"
#include "ObjectHandle.h"

#include "Join.h"

#include "Serialization.h"

namespace Atmos
{
    class RenderFragmentHandler;
    class RenderFragment : public Sense
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend RenderFragmentHandler;
    private:
        Size3D size;
        AxisBoundingBox3D bounds;

        void CalculateBounds();

        void OnPositionChanged() override;
        void OnEnabledChanged() override;

        virtual void DrawImpl() const = 0;
        virtual AxisBoundingBox3D::Coordinate GetZHeight() const = 0;
    protected:
        RenderFragment(bool enabled);

        void SetWidth(Size3D::ValueT set);
        void SetHeight(Size3D::ValueT set);
        void SetDepth(Size3D::ValueT set);
        void SetScalers(const Join3<Size3D::ValueT> &set);
        void SetXScaler(Size3D::ValueT set);
        void SetYScaler(Size3D::ValueT set);
        void SetZScaler(Size3D::ValueT set);
    public:
        ObjectHandle<RenderFragmentHandler, RenderFragment> handler;

        RenderFragment(const RenderFragment &arg);
        RenderFragment(RenderFragment &&arg);
        RenderFragment& operator=(const RenderFragment &arg);
        RenderFragment& operator=(RenderFragment &&arg);
        virtual ~RenderFragment() = 0;
        bool operator==(const RenderFragment &arg) const;
        bool operator!=(const RenderFragment &arg) const;

        void Draw();

        const Size3D& GetSize() const;
        Size3D::ValueT GetWidth() const;
        Size3D::ValueT GetHeight() const;
        Size3D::ValueT GetDepth() const;
        Join3<Size3D::ValueT> GetScalers() const;
        Size3D::ValueT GetXScaler() const;
        Size3D::ValueT GetYScaler() const;
        Size3D::ValueT GetZScaler() const;

        void SetXRotation(const Angle &set);
        void SetYRotation(const Angle &set);
        void SetZRotation(const Angle &set);
        const Angle& GetXRotation() const;
        const Angle& GetYRotation() const;
        const Angle& GetZRotation() const;

        const AxisBoundingBox3D& GetBounds() const;
    };
}