#pragma once

#include <Arca/Curator.h>

#include "ScriptManager.h"
#include "Script.h"
#include "ScriptResult.h"

#include "Work.h"
#include "SuspendScript.h"
#include "CompileScript.h"
#include "ModifyScriptData.h"
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
        void Handle(const Suspend& command);
        File::Path Handle(const Compile& command);
        void Handle(const ModifyData& command);

        std::unique_ptr<Asset::Resource::Script> Handle(
            const Asset::Resource::Create<Asset::Resource::Script>& command);
    private:
        Manager* manager;

        std::optional<Result> DoExecute(Script* script);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Scripting::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Scripting::Suspend,
            Atmos::Scripting::Compile,
            Atmos::Scripting::ModifyData,
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Script>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Scripting::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Scripting::Curator>;
    };
}
