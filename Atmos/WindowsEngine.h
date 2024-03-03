#pragma once

#include "Engine.h"
#include "WindowsInclude.h"

namespace Atmos
{
    class WindowsEngine : public Engine
    {
    public:
        WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, const String& className);
    protected:
        CreatedContext CreateDefaultContext() override;

        void DoExit() override;
    private:
        HINSTANCE hInstance;
        LPSTR lpCmdLine;
        int nCmdShow;
        String className;
    };
}