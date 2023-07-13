#pragma once

#include <Arca/Curator.h>

#include "ScriptManager.h"
#include "Script.h"
#include "ScriptResult.h"

#include "Work.h"
#include "CompileScript.h"
#include "ExecuteScript.h"
#include "ExecuteMaterial.h"
#include "CreateScriptAssetResource.h"

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
        std::vector<CompiledModule> Handle(const Compile& command);
        std::optional<Result> Handle(const Execute& command);
        std::optional<Result> Handle(const ExecuteMaterial& command);

        std::unique_ptr<Asset::Resource::Script> Handle(
            const Asset::Resource::Create<Asset::Resource::Script>& command);
    private:
        Manager* manager;
    private:
        const ScriptData* current = nullptr;

        struct RunningScript
        {
            Arca::RelicID id = Arca::nullRelicID;
            Script* script = nullptr;
        };

        [[nodiscard]] std::vector<RunningScript> RunningScripts(const Arca::Batch<Script>& batch);

        std::optional<Result> DoExecute(ScriptData* script);
        std::optional<Result> HandleScriptResult(const std::optional<Result>& result, Arca::RelicID id);
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
            Atmos::Scripting::Compile,
            Atmos::Scripting::Execute,
            Atmos::Scripting::ExecuteMaterial,
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
