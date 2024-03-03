#pragma once

#include <Arca/Curator.h>

#include "RunningScript.h"
#include "ScriptInstance.h"
#include "AllRunningScripts.h"

namespace Atmos::Debug
{
    class Statistics;
}

namespace Atmos::Script
{
    class ScriptController final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        AllRunningScripts* allRunningScripts = nullptr;
    private:
        void LaunchOrRunScript(RunningScript& script);
    private:
        Debug::Statistics* debugStatistics = nullptr;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ScriptController>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::ScriptController, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Script::ScriptController, BinaryArchive>
    {};
}
