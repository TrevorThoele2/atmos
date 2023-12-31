#pragma once

#include <Arca/Curator.h>

#include "ScriptManager.h"
#include "Script.h"
#include "ScriptResult.h"

#include "Work.h"
#include "SuspendScript.h"
#include "CompileScript.h"
#include "ExecuteScript.h"
#include "CreateScriptAssetResource.h"
#include "CurrentExecutingScript.h"

class asIScriptFunction;
class asIScriptContext;

namespace Atmos::Scripting
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, Manager& manager);
    public:
        void Handle(const Work& command);
        void Handle(const Suspend& command);
        std::vector<CompiledModule> Handle(const Compile& command);
        std::optional<Result> Handle(const Execute& command);

        std::unique_ptr<Asset::Resource::Script> Handle(
            const Asset::Resource::Create<Asset::Resource::Script>& command);
    private:
        Manager* manager;
    private:
        struct RunningScript
        {
            Arca::RelicID id = Arca::nullRelicID;
            const Script* script = nullptr;
            RunningScript(Arca::RelicID id, const Script* script);
            RunningScript(const RunningScript& arg);
        };

        [[nodiscard]] static std::vector<RunningScript> ScriptsToRunning(Arca::Batch<Script> batch);

        std::optional<Result> DoExecute(Arca::RelicID id, CurrentExecutingScript& currentExecutingScript);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Curator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Scripting::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Scripting::Suspend,
            Atmos::Scripting::Compile,
            Atmos::Scripting::Execute,
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Script>>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Scripting::Curator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Scripting::Curator>;
    };
}
