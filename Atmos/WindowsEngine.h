#pragma once

#include "Engine.h"
#include "WindowsInclude.h"

namespace Atmos
{
    class WindowsEngine final : public Engine
    {
    public:
        WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, String className, Logging::Logger& logger);
    protected:
        void SetupImplementation() override;

        InitializationProperties CreateInitializationProperties(Logging::Logger& logger) override;

        void DoExit() override;
    private:
        HINSTANCE hInstance;
        LPSTR lpCmdLine;
        int nCmdShow;
        String className;
    };
}