#pragma once

#include "Environment.h"

namespace Atmos
{
    class NullEnvironment : public EnvironmentModel
    {
    private:
        void SetupImpl() override;
        AxisBoundingBox2D AdjustWindowDimensions() override;
        void OnSetWindowDimensions() override;
        Position2D GetDefaultWindowPosition() override;
        void OnSetFullscreen() override;
    public:
        void Show() override;
        bool StartFrame() override;
        void Exit() override;
        bool IsFocused() const override;

        void CreateInput() override;
        void CreateGraphics() override;
        void CreateAudio() override;
        void CreateFileSystem() override;
    };
}