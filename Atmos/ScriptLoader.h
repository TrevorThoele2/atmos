#pragma once

#include <memory>

namespace Falcon
{
    class ModuleLoader;
    class Module;
}

namespace Atmos
{
    class ScriptLoader
    {
    private:
        class Impl;
        std::unique_ptr<Impl> impl;

        ScriptLoader();
    public:
        static ScriptLoader& Instance();
        static Falcon::ModuleLoader& GetModuleLoader();
        static Falcon::Module* GetMainModule();
    };
}