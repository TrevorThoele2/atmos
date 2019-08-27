#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "Position2D.h"
#include "Position3D.h"
#include "Size2D.h"
#include "AxisAlignedBox2D.h"

namespace Atmos::Render
{
    class Camera final : public Arca::ClosedTypedRelicAutomation<Camera>
    {
    public:
        Camera() = default;

        void FocusedPosition(const Position3D* to);
        [[nodiscard]] const Position3D* FocusedPosition() const;

        void ViewOrigin(const Position2D& to);
        [[nodiscard]] Position2D ViewOrigin() const;
        void Size(const Size2D& to);
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D ScreenSides() const;

        void Zoom(Position3D::Value to);
        [[nodiscard]] Position3D::Value Zoom() const;
    private:
        const Position3D* focusedPosition = nullptr;

        // View origin is the middle of the screen
        Position2D viewOrigin;
        Size2D size;
        AxisAlignedBox2D screenSides;

        Position2D topLeft;

        Position3D::Value zoom = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::Camera>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::Camera, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::Camera, BinaryArchive>
    {};
}