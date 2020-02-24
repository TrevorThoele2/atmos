#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "Position2D.h"
#include "Position3D.h"
#include "ScreenSize.h"
#include "AxisAlignedBox2D.h"

namespace Atmos::Render
{
    class Camera final : public Arca::ClosedTypedRelic<Camera>
    {
    public:
        const Position3D* focusedPosition = nullptr;

        Position2D center;
        ScreenSize size;

        Position3D::Value zoom = 0;

        [[nodiscard]] AxisAlignedBox2D ScreenSides() const;
    public:
        explicit Camera(Init init);
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::Camera>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Camera";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::Camera, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Render::Camera, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}