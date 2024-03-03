#pragma once

#include <Arca/Curator.h>

#include "Camera.h"

#include "MoveCamera.h"
#include "MoveCameraBy.h"
#include "MoveCameraTo.h"
#include "CameraMoved.h"

#include "ResizeCamera.h"
#include "CameraResized.h"
#include "ReconstructGraphics.h"

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
        void Handle(const MoveCameraTo& command);
        void Handle(const ResizeCamera& command);
    private:
        Arca::Index<Camera> camera;
    private:
        void Move(Direction direction, Position3D::Value by);
        void MoveBy(Position3D::Value x, Position3D::Value y);
        void MoveTo(const Position3D& position);
        template<class Function>
        void DoMove(Function function);

        void Resize(const ScreenSize& size);
        template<class Function>
        void DoResize(Function function);
    private:
        Debug::Value debugCenterX;
        Debug::Value debugCenterY;
    };

    template<class Function>
    void CameraCurator::DoMove(Function function)
    {
        const auto previousCenter = camera->center;
        function();
        if (previousCenter != camera->center)
            Owner().Raise<CameraMoved>(camera);
    }

    template<class Function>
    void CameraCurator::DoResize(Function function)
    {
        const auto previousSize = camera->size;
        function();
        if (previousSize != camera->size)
        {
            Owner().Raise<CameraResized>(camera);
            Owner().Do<ReconstructGraphics>();
        }
    }
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
            Atmos::Render::MoveCameraTo,
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