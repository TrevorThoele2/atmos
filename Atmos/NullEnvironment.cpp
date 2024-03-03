
#include "NullEnvironment.h"

#include "NullInput.h"
#include "NullGraphics.h"
#include "NullAudio.h"
#include "FileSystemWindows.h"

namespace Atmos
{
    void NullEnvironment::SetupImpl()
    {}

    AxisBoundingBox2D NullEnvironment::AdjustWindowDimensions()
    {
        return AxisBoundingBox2D();
    }

    void NullEnvironment::OnSetWindowDimensions()
    {}

    Position2D NullEnvironment::GetDefaultWindowPosition()
    {
        return Position2D();
    }

    void NullEnvironment::OnSetFullscreen()
    {}

    void NullEnvironment::Show()
    {}

    bool NullEnvironment::StartFrame()
    {
        return true;
    }

    void NullEnvironment::Exit()
    {}

    bool NullEnvironment::IsFocused() const
    {
        return true;
    }

    void NullEnvironment::CreateInput()
    {
        Environment::CreateInputHandler<Input::NullHandler>();
    }

    void NullEnvironment::CreateGraphics()
    {
        Environment::CreateGraphicsHandler<NullGraphicsHandler>();
    }

    void NullEnvironment::CreateAudio()
    {
        Environment::CreateAudioHandler<NullAudioHandler>();
    }

    void NullEnvironment::CreateFileSystem()
    {
        Environment::CreateFileSystem<FileSystem::WindowsHandler>();
    }
}