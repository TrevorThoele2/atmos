#pragma once

#include <Arca/Curator.h>

#include "RunningScript.h"
#include "ScriptInstance.h"
#include "AllRunningScripts.h"

#include "ExecuteImmediately.h"
#include "ForceQuit.h"

namespace Atmos::Script
{
    class ScriptController final : public Arca::Curator
    {
    public:
        explicit ScriptController(Init init);

        void Work();
    public:
        void Handle(const ExecuteImmediately& command);
        void Handle(const ForceQuit& command);
    private:
        Arca::Index<AllRunningScripts> allRunningScripts;
    private:
        void LaunchOrRunScript(RunningScript& script);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ScriptController>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "ScriptController";
        using HandledCommands = HandledCommands<
            Atmos::Script::ExecuteImmediately,
            Atmos::Script::ForceQuit>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::ScriptController, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Script::ScriptController, BinaryArchive>
    {};
}
