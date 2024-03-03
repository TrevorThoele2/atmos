#pragma once

#include "TimeValue.h"
#include "AxisAlignedBox2D.h"

#include "ScreenPosition.h"
#include "ScreenDimensions.h"

namespace Atmos
{
    class ObjectManager;
    class LoggingSystem;

    namespace Render
    {
        class GraphicsSystem;
    }
}

namespace Atmos::Window
{
    class WindowBase
    {
    public:
        typedef ScreenPosition Position;
        typedef ScreenDimensions Size;
    public:
        virtual ~WindowBase() = 0;

        virtual void Setup() = 0;

        virtual void Show() = 0;
        virtual void Exit() = 0;
        virtual bool IsCurrentlyFocused() const = 0;
        virtual void Suspend(const Time::Value& time) = 0;
        virtual bool OnStartFrame() = 0;

        void ToggleFullscreen();
        void SetFullscreen(bool set = true);
        bool IsFullscreen() const;
        bool IsWindowed() const;

        Size DefaultSize();
        Size ClientSize();
        Size WindowSize();
        Position StartPosition();
    protected:
        WindowBase(ObjectManager& objectManager);
    protected:
        void SetWindowDimensions();

        virtual AxisAlignedBox2D AdjustWindowDimensions() = 0;
        virtual void OnSetWindowDimensions() = 0;
        virtual Position GetDefaultWindowPosition() = 0;
        virtual void OnSetFullscreen() = 0;
    protected:
        ObjectManager* GetObjectManager();
        const ObjectManager* GetObjectManager() const;
        LoggingSystem* FindLoggingSystem();
    private:
        Size clientSize;
        Size windowSize;
        Position startPosition;
    private:
        bool isFocusLost;
        bool isFullscreen;
    private:
        ObjectManager* objectManager;
        Render::GraphicsSystem* FindGraphicsSystem();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Window::WindowBase, BinaryArchive> :
        public TableScribe<::Atmos::Window::WindowBase, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
        };
    };
}