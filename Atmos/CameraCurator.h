#pragma once

#include <Arca/Curator.h>

#include "MoveCamera.h"
#include "MoveCameraBy.h"
#include "MoveCameraToInstant.h"
#include "MoveCameraDeltaInstant.h"
#include "DebugStatistics.h"

namespace Atmos::Render
{
    class Camera;

    class CameraCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        Camera* camera = nullptr;

        // Holder for any potential positions that aren't stored elsewhere (just looking somewhere, etc)
        Position3D basePosition;
    private:
        void Move(Direction direction, Position3D::Value by);
        void MoveBy(Position3D::Value x, Position3D::Value y, Position3D::Value z);
        void MoveToInstant(const Position3D& pos);
        void MoveDeltaInstant(Position3D::Value x, Position3D::Value y, Position3D::Value z);
    private:
        void ResetFocus();

        void CalculateSides();
        bool IsFocusValid();
    private:
        Debug::Statistics* debugStatistics = nullptr;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::CameraCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::CameraCurator, BinaryArchive>
    {};
}