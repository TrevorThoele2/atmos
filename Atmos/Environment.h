
#pragma once

#include <string>

#include "IniManager.h"
#include "Flags.h"
#include "FilePath.h"
#include "Join.h"

#include "InputHandlerBase.h"
#include "GraphicsHandlerBase.h"
#include "AudioHandlerBase.h"
#include "FileSystem.h"
#include "TimeHandler.h"

#include "Position2D.h"
#include "AxisBoundingBox2D.h"
#include "MousePosition.h"

#include "Logger.h"
#include <AGUI/Resolution.h>

namespace Atmos
{
    class EnvironmentModel
    {
    public:
        typedef unsigned int DimensionT;
    private:
        class Dimension
        {
        private:
            DimensionT clientLength, totalLength, start;
        public:
            Dimension();
            void SetClientLength(DimensionT set);
            void SetTotalLength(DimensionT set);
            void SetStart(DimensionT systemLength);
            DimensionT GetClientLength() const;
            DimensionT GetTotalLength() const;
            DimensionT GetStart() const;
        };
    public:
        enum class Flag
        {
            FOCUS_LOST = 1 << 0,
            WINDOWED = 1 << 1,

            DEFAULT = WINDOWED
        };
    private:
        Flags<Flag> flags;
        Dimension x, y;

        virtual void SetupImpl() = 0;
        void SetClientDimensions(DimensionT width, DimensionT height);
        virtual AxisBoundingBox2D AdjustWindowDimensions() = 0;
        virtual void OnSetWindowDimensions() = 0;
        virtual Position2D GetDefaultWindowPosition() = 0;

        void OnFocusLost();
        void OnFocusRegain();
        void OnResolutionChanged(const Agui::Resolution &arg);
        virtual void OnSetFullscreen() = 0;
    protected:
        void SetWindowDimensions();
        bool GetFlag(Flag get) const;
        const Dimension& GetDimensionX() const;
        const Dimension& GetDimensionY() const;
    public:
        virtual ~EnvironmentModel() = 0 {}
        void Setup();

        virtual void Show() = 0;
        virtual bool StartFrame() = 0;
        virtual void Exit() = 0;
        virtual bool IsFocused() const = 0;

        // Creates the input handler and adds it to the environment
        virtual void CreateInput() = 0;
        // Creates the graphics handler and adds it to the environment
        virtual void CreateGraphics() = 0;
        // Creates the audio handler and adds it to the environment
        virtual void CreateAudio() = 0;
        // Creates the file system handler and adds it to the environment
        virtual void CreateFileSystem() = 0;

        void ToggleFullscreen();
        void SetFullscreen(bool set = true);
        Join2<DimensionT>& GetDefaultSize();
        DimensionT GetClientWidth();
        DimensionT GetClientHeight();
        DimensionT GetWindowWidth();
        DimensionT GetWindowHeight();
        DimensionT GetStartX();
        DimensionT GetStartY();

        bool GetFlag(Flag flag);
        static const String& GetVersion();
    };

    class Environment
    {
    public:
        typedef EnvironmentModel::DimensionT DimensionT;
        typedef EnvironmentModel::Flag Flag;
    private:
        static Environment instance;
        static std::unique_ptr<EnvironmentModel> model;
        static std::unique_ptr<Input::HandlerBase> input;
        static std::unique_ptr<GraphicsHandlerBase> graphics;
        static std::unique_ptr<AudioHandlerBase> audio;
        static std::unique_ptr<FileSystem::Handler> fileSystem;
        static TimeHandler time;
        static Ini::Manager iniManager;
        static bool wasFocused;

        Environment() = default;
        Environment(const Environment &arg) = delete;
        Environment& operator=(const Environment &arg) = delete;

        static void SetupBasic();
    public:
        static NullEvent eventFocusLost;
        static NullEvent eventFocusRegain;

        static Environment& Instance();

        template<class Model, class... Args>
        static void Setup(Args && ... args);
        template<class Handler, class... Args>
        static void CreateInputHandler(Args && ... args);
        template<class Handler, class... Args>
        static void CreateGraphicsHandler(Args && ... args);
        template<class Handler, class... Args>
        static void CreateAudioHandler(Args && ... args);
        template<class RealT, class... Args>
        static void CreateFileSystem(Args && ... args);

        template<class Model>
        static Model* GetModel();
        static EnvironmentModel* GetModel();
        template<class Model>
        static bool IsModel();

        // Does not check if the Handler type is correct
        template<class Handler>
        static Handler* GetInput();
        static Input::HandlerBase* GetInput();
        template<class Handler>
        static bool IsInput();
        static bool IsInputOccupied();
        // Does not check if the Handler type is correct
        template<class Handler>
        static Handler* GetGraphics();
        static GraphicsHandlerBase* GetGraphics();
        template<class Handler>
        static bool IsGraphics();
        static bool IsGraphicsOccupied();
        // Does not check if the Handler type is correct
        template<class Handler>
        static Handler* GetAudio();
        static AudioHandlerBase* GetAudio();
        template<class Handler>
        static bool IsAudio();
        static bool IsAudioOccupied();
        template<class RealT>
        static RealT* GetFileSystem();
        static FileSystem::Handler* GetFileSystem();
        template<class RealT>
        static bool IsFileSystem();
        static bool IsFileSystemOccupied();
        static TimeHandler& GetTime();
        static Ini::Manager& GetIni();

        static void StartExecution();

        static void Show();
        static bool StartFrame();
        static void Exit();
        static bool IsFocused();
        static void Suspend(const TimeValue &time);

        static void ToggleFullscreen();
        static void SetFullscreen(bool set = true);
        static Join2<DimensionT>& GetDefaultSize();
        static DimensionT GetClientWidth();
        static DimensionT GetClientHeight();
        static DimensionT GetWindowWidth();
        static DimensionT GetWindowHeight();
        static DimensionT GetStartX();
        static DimensionT GetStartY();

        static bool GetFlag(Flag flag);
        static const String& GetVersion();
    };

    template<class Model, class... Args>
    void Environment::Setup(Args && ... args)
    {
        model.reset(new Model(std::forward<Args>(args)...));
        model->Setup();
        SetupBasic();
    }

    template<class Handler, class... Args>
    void Environment::CreateInputHandler(Args && ... args)
    {
        input.reset(new Handler(std::forward<Args>(args)...));
    }

    template<class Handler, class... Args>
    void Environment::CreateGraphicsHandler(Args && ... args)
    {
        graphics.reset(new Handler(std::forward<Args>(args)...));
    }

    template<class Handler, class... Args>
    void Environment::CreateAudioHandler(Args && ... args)
    {
        audio.reset(new Handler(std::forward<Args>(args)...));
    }

    template<class RealT, class... Args>
    void Environment::CreateFileSystem(Args && ... args)
    {
        fileSystem.reset(new RealT(std::forward<Args>(args)...));
    }

    template<class Model>
    Model* Environment::GetModel()
    {
        return static_cast<Model*>(model.get());
    }

    template<class Model>
    bool Environment::IsModel()
    {
        return typeid(model.get()) == typeid(Model*);
    }

    template<class Handler>
    Handler* Environment::GetInput()
    {
        return static_cast<Handler*>(input.get());
    }

    template<class Handler>
    bool Environment::IsInput()
    {
        return dynamic_cast<Handler*>(input.get());
    }

    template<class Handler>
    Handler* Environment::GetGraphics()
    {
        return static_cast<Handler*>(graphics.get());
    }

    template<class Handler>
    bool Environment::IsGraphics()
    {
        return dynamic_cast<Handler*>(graphics.get());
    }

    template<class Handler>
    Handler* Environment::GetAudio()
    {
        return static_cast<Handler*>(audio.get());
    }

    template<class Handler>
    bool Environment::IsAudio()
    {
        return typeid(audio.get()) == typeid(Handler*);
    }

    template<class RealT>
    RealT* Environment::GetFileSystem()
    {
        return static_cast<RealT*>(fileSystem.get());
    }

    template<class RealT>
    bool Environment::IsFileSystem()
    {
        return typeid(fileSystem.get()) == typeid(RealT*);
    }
}