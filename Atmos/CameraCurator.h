#pragma once

#include <Arca/Curator.h>

#include "Camera.h"
#include "MoveCamera.h"
#include "MoveCameraBy.h"
#include "MoveCameraToInstant.h"
#include "MoveCameraDeltaInstant.h"
#include "ResizeCamera.h"

#include "DebugValue.h"

namespace Atmos::Render
{
    class CameraCurator final : public Arca::Curator
    {
    public:
        explicit CameraCurator(Init init);

        void Work();
    public:
        void Handle(const MoveCamera& command);
        void Handle(const MoveCameraBy& command);
        void Handle(const MoveCameraToInstant& command);
        void Handle(const MoveCameraDeltaInstant& command);
        void Handle(const ResizeCamera& command);
    private:
        Arca::Index<Camera> camera;

        // Holder for any potential positions that aren't stored elsewhere (just looking somewhere, etc)
        Position3D basePosition;
    private:
        void Move(Direction direction, Position3D::Value by);
        void MoveBy(Position3D::Value x, Position3D::Value y, Position3D::Value z);
        void MoveToInstant(const Position3D& pos);
        void MoveDeltaInstant(Position3D::Value x, Position3D::Value y, Position3D::Value z);
        void Resize(const ScreenSize& size);
    private:
        void ResetFocus();

        void CalculateSides();
        bool IsFocusValid();
    private:
        Debug::Value debugViewOriginX;
        Debug::Value debugViewOriginY;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CameraCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "CameraCurator";
        using HandledCommands = Arca::HandledCommands<
            Atmos::Render::MoveCamera,
            Atmos::Render::MoveCameraBy,
            Atmos::Render::MoveCameraDeltaInstant,
            Atmos::Render::MoveCameraToInstant,
            Atmos::Render::ResizeCamera>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::CameraCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::CameraCurator, BinaryArchive>
    {};
}