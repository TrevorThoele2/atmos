#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "ScriptAsset.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

#include "NameValuePair.h"

namespace Atmos::Script
{
    class RunningScript;

    class ScriptInstance final : public Arca::ClosedTypedRelicAutomation<ScriptInstance>
    {
    public:
        using Asset = Asset::ScriptAsset;
        Asset* asset;
    public:
        Name executeName;
        Parameters parameters;
        Persistence persistence;
    public:
        // Executes the script standardly
        // Will defer the execute until a certain point in the frame (probably a bit later)
        void ExecuteDeferred();
        // Executes the script standardly
        // Will immediately execute the script
        void ExecuteImmediately();
        [[nodiscard]] RunningScript* RunningForThis() const;
        // Returns if this script is currently running
        // NOTE: Suspension has no effect on this
        [[nodiscard]] bool IsRunning() const;
    private:
        RunningScript& CreateRunningFromThis();
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Script::ScriptInstance>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "ScriptInstance";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::ScriptInstance, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Script::ScriptInstance, BinaryArchive>
    {};
}