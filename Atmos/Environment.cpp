
#include "Environment.h"

#include "GameEnvironment.h"
#include "Input.h"
#include "FPS.h"
#include "StasisScribe.h"
#include "IniID.h"

#include "MasterSoundHandler.h"
#include "DXGraphics.h"

#include <AGUI/System.h>
#include <AGUI/DXGraphics.h>
#include "WidgetSetups.h"

namespace Atmos
{
    EnvironmentModel::Dimension::Dimension() : clientLength(0), totalLength(0), start(0)
    {}

    void EnvironmentModel::Dimension::SetClientLength(DimensionT set)
    {
        clientLength = set;
    }

    void EnvironmentModel::Dimension::SetTotalLength(DimensionT set)
    {
        totalLength = set;
    }

    void EnvironmentModel::Dimension::SetStart(DimensionT systemLength)
    {
        if (systemLength >= totalLength)
            start = (systemLength - totalLength) / 2;
        else
            start = 0;
    }

    EnvironmentModel::DimensionT EnvironmentModel::Dimension::GetClientLength() const
    {
        return clientLength;
    }

    EnvironmentModel::DimensionT EnvironmentModel::Dimension::GetTotalLength() const
    {
        return totalLength;
    }

    EnvironmentModel::DimensionT EnvironmentModel::Dimension::GetStart() const
    {
        return start;
    }

    void EnvironmentModel::SetClientDimensions(DimensionT width, DimensionT height)
    {
        x.SetClientLength(width);
        y.SetClientLength(height);
        SetWindowDimensions();
    }

    void EnvironmentModel::OnFocusLost()
    {
        flags.Set(Flag::FOCUS_LOST);
    }

    void EnvironmentModel::OnFocusRegain()
    {
        flags.Set(Flag::FOCUS_LOST, false);
    }

    void EnvironmentModel::OnResolutionChanged(const agui::Resolution &arg)
    {
        SetClientDimensions(arg.GetSize().width, arg.GetSize().height);
    }

    void EnvironmentModel::SetWindowDimensions()
    {
        // Set lengths
        {
            AxisBoundingBox2D box = AdjustWindowDimensions();
            x.SetTotalLength(static_cast<DimensionT>(abs(box.GetLeft()) + abs(box.GetRight())));
            y.SetTotalLength(static_cast<DimensionT>(abs(box.GetTop()) + abs(box.GetBottom())));
        }

        // Set default position
        {
            Position2D defaultPosition = GetDefaultWindowPosition();
            x.SetStart(static_cast<DimensionT>(defaultPosition.GetX()));
            y.SetStart(static_cast<DimensionT>(defaultPosition.GetY()));
        }

        if(Environment::IsGraphicsOccupied())
            Environment::GetGraphics()->SetMainDimensions(GraphicsHandlerBase::ScreenDimensions(x.GetClientLength(), y.GetClientLength()));
        OnSetWindowDimensions();
    }

    bool EnvironmentModel::GetFlag(Flag get) const
    {
        return flags.Get(get);
    }

    const EnvironmentModel::Dimension& EnvironmentModel::GetDimensionX() const
    {
        return x;
    }

    const EnvironmentModel::Dimension& EnvironmentModel::GetDimensionY() const
    {
        return y;
    }

    void EnvironmentModel::Setup()
    {
        CreateFileSystem();
        Environment::GetFileSystem()->Init();
        CreateInput();
        Environment::GetInput()->InitCreateInputs();
        Environment::GetIni().Init();
        Environment::GetIni().Load();
        Environment::GetInput()->InitCreateActions();
        Environment::GetInput()->InitFinalize();

        FpsHandler::Init();

        agui::System::Instance().eventResolutionChanged.Subscribe(std::bind(&EnvironmentModel::OnResolutionChanged, this, std::placeholders::_1));
        Environment::eventFocusLost.Subscribe(&EnvironmentModel::OnFocusLost, *this);
        Environment::eventFocusRegain.Subscribe(&EnvironmentModel::OnFocusRegain, *this);
        flags.Set(Flag::WINDOWED, *Environment::GetIni().GetEntry<Ini::ID::WINDOWED>());
        SetClientDimensions(GetDefaultSize().first, GetDefaultSize().second);
        SetupImpl();

        CreateGraphics();
        CreateAudio();
    }

    void EnvironmentModel::ToggleFullscreen()
    {
        SetFullscreen(!flags.Get(Flag::WINDOWED));
    }

    void EnvironmentModel::SetFullscreen(bool set)
    {
        if (flags.Get(Flag::WINDOWED) == !set)
            return;

        flags.Set(Flag::WINDOWED, !set);
        OnSetFullscreen();
        Environment::GetGraphics()->SetFullscreen(!flags.Get(Flag::WINDOWED));
        Environment::GetGraphics()->Reinitialize();
    }

    Join2<EnvironmentModel::DimensionT>& EnvironmentModel::GetDefaultSize()
    {
        static Join2<DimensionT> pos(1024, 768);
        return pos;
    }

    EnvironmentModel::DimensionT EnvironmentModel::GetClientWidth()
    {
        return x.GetClientLength();
    }

    EnvironmentModel::DimensionT EnvironmentModel::GetClientHeight()
    {
        return y.GetClientLength();
    }

    EnvironmentModel::DimensionT EnvironmentModel::GetWindowWidth()
    {
        return x.GetTotalLength();
    }

    EnvironmentModel::DimensionT EnvironmentModel::GetWindowHeight()
    {
        return y.GetTotalLength();
    }

    EnvironmentModel::DimensionT EnvironmentModel::GetStartX()
    {
        return x.GetStart();
    }

    EnvironmentModel::DimensionT EnvironmentModel::GetStartY()
    {
        return y.GetStart();
    }

    bool EnvironmentModel::GetFlag(Flag flag)
    {
        return flags.Get(flag);
    }

    const std::string& EnvironmentModel::GetVersion()
    {
        static std::string ret("VERSION: 0.9.0 (4/28/2018)");
        return ret;
    }

    Environment Environment::instance;
    std::unique_ptr<EnvironmentModel> Environment::model;
    std::unique_ptr<Input::HandlerBase> Environment::input;
    std::unique_ptr<GraphicsHandlerBase> Environment::graphics;
    std::unique_ptr<AudioHandlerBase> Environment::audio;
    std::unique_ptr<FileSystem::Handler> Environment::fileSystem;
    TimeHandler Environment::time;
    Ini::Manager Environment::iniManager;
    bool Environment::wasFocused = true;

    void Environment::SetupBasic()
    {
        Logger::Init();

        // Sound and music
        MasterSoundHandler::Init();

        // GUI
        agui::System::Initialize(agui::Resolution::Size(Environment::GetDefaultSize().first, Environment::GetDefaultSize().second), new agui::DX9GraphicsHandler(GetGraphics<DX9GraphicsHandler>()->GetDevice()));
        // Image resources
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\Pixel.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\button.png").GetValue(), 3, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\buttonSmall.png").GetValue(), 3, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\buttonSmaller.png").GetValue(), 3, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\check24.png").GetValue(), 2, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\SliderBackground.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\SliderBackgroundVertical.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\SliderMovable.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\FillbarBackground.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\FillbarInside.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\cursor.png").GetValue());
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\item.png").GetValue(), 16, 16);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\increment.png").GetValue(), 3, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\decrement.png").GetValue(), 3, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\statusFrame.png").GetValue(), 2, 1);
        agui::System::CreateImageResource(Environment::GetFileSystem()->GetExePath().Append("Graphics\\GUI\\statusFrameSpell.png").GetValue());

        // Fonts
        agui::fontSlender = agui::System::CreateFont("slender", 12, 0, 300, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, "Arial");
        agui::fontButton = agui::System::CreateFont("button", 12, 0, 600, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, "Arial");

        // RESOLUTIONS
        // 4:3
        agui::System::CreateRes(agui::Resolution::Size(1024, 768));
        agui::System::CreateRes(agui::Resolution::Size(1152, 864));
        agui::System::CreateRes(agui::Resolution::Size(1280, 960));
        agui::System::CreateRes(agui::Resolution::Size(1400, 1050));
        agui::System::CreateRes(agui::Resolution::Size(1440, 1080));
        agui::System::CreateRes(agui::Resolution::Size(1600, 1200));
        agui::System::CreateRes(agui::Resolution::Size(2048, 1536));

        // 16:9
        agui::System::CreateRes(agui::Resolution::Size(1024, 576));
        agui::System::CreateRes(agui::Resolution::Size(1280, 720));
        agui::System::CreateRes(agui::Resolution::Size(1366, 768), new agui::Resolution::AspectRatio(agui::Ratio(16, 9)));
        agui::System::CreateRes(agui::Resolution::Size(1600, 900));
        agui::System::CreateRes(agui::Resolution::Size(1920, 1080));
        agui::System::CreateRes(agui::Resolution::Size(2560, 1440));

        // 16:10
        agui::System::CreateRes(agui::Resolution::Size(1280, 800), new agui::Resolution::AspectRatio(agui::Ratio(16, 10), false));
        agui::System::CreateRes(agui::Resolution::Size(1440, 900), new agui::Resolution::AspectRatio(agui::Ratio(16, 10), false));
        agui::System::CreateRes(agui::Resolution::Size(1680, 1050), new agui::Resolution::AspectRatio(agui::Ratio(16, 10), false));
        agui::System::CreateRes(agui::Resolution::Size(1920, 1200), new agui::Resolution::AspectRatio(agui::Ratio(16, 10), false));
        agui::System::CreateRes(agui::Resolution::Size(2560, 1600), new agui::Resolution::AspectRatio(agui::Ratio(16, 10), false));

        agui::System::SetRes(*GetIni().GetEntry<Ini::ID::RESOLUTION>());

        // GUI defaults
        agui::SetupWidgets();

        GameEnvironment::Init();
    }

    NullEvent Environment::eventFocusLost;
    NullEvent Environment::eventFocusRegain;

    Environment& Environment::Instance()
    {
        static Environment instance;
        return instance;
    }

    EnvironmentModel* Environment::GetModel()
    {
        return model.get();
    }

    Input::HandlerBase* Environment::GetInput()
    {
        return input.get();
    }

    bool Environment::IsInputOccupied()
    {
        return input.get() != nullptr;
    }

    GraphicsHandlerBase* Environment::GetGraphics()
    {
        return graphics.get();
    }

    bool Environment::IsGraphicsOccupied()
    {
        return graphics.get() != nullptr;
    }

    AudioHandlerBase* Environment::GetAudio()
    {
        return audio.get();
    }

    bool Environment::IsAudioOccupied()
    {
        return audio.get() != nullptr;
    }

    FileSystem::Handler* Environment::GetFileSystem()
    {
        return fileSystem.get();
    }

    bool Environment::IsFileSystemOccupied()
    {
        return fileSystem.get() != nullptr;
    }

    TimeHandler& Environment::GetTime()
    {
        return time;
    }

    Ini::Manager& Environment::GetIni()
    {
        return iniManager;
    }

    void Environment::StartExecution()
    {
        GameEnvironment::Loop();
    }

    void Environment::Show()
    {
        model->Show();
    }

    bool Environment::StartFrame()
    {
        // Check for and emit focus events
        if (IsFocused() != wasFocused)
        {
            (wasFocused) ? eventFocusLost() : eventFocusRegain();
            wasFocused = !wasFocused;
        }

        return model->StartFrame();
    }

    void Environment::Exit()
    {
        model->Exit();
    }

    bool Environment::IsFocused()
    {
        return model->IsFocused();
    }

    void Environment::Suspend(const TimeValue &time)
    {
        auto test = static_cast<unsigned int>(time.ConvertValue(TimeValueEpoch::MILLISECONDS));
        Sleep(test);
    }

    void Environment::ToggleFullscreen()
    {
        model->ToggleFullscreen();
    }

    void Environment::SetFullscreen(bool set)
    {
        model->SetFullscreen(set);
    }

    Join2<Environment::DimensionT>& Environment::GetDefaultSize()
    {
        return model->GetDefaultSize();
    }

    Environment::DimensionT Environment::GetClientWidth()
    {
        return model->GetClientWidth();
    }

    Environment::DimensionT Environment::GetClientHeight()
    {
        return model->GetClientHeight();
    }

    Environment::DimensionT Environment::GetWindowWidth()
    {
        return model->GetWindowWidth();
    }

    Environment::DimensionT Environment::GetWindowHeight()
    {
        return model->GetWindowHeight();
    }

    Environment::DimensionT Environment::GetStartX()
    {
        return model->GetStartX();
    }

    Environment::DimensionT Environment::GetStartY()
    {
        return model->GetStartY();
    }

    bool Environment::GetFlag(Flag flag)
    {
        return model->GetFlag(flag);
    }

    const String& Environment::GetVersion()
    {
        return model->GetVersion();
    }
}