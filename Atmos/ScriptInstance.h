#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "ScriptAsset.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

#include "NameValuePair.h"

namespace Atmos::Script
{
    class RunningScript;

    class Instance final : public Arca::ClosedTypedRelic<Instance>
    {
    public:
        using Asset = Asset::Script;
        Asset* asset;
    public:
        Name executeName;
        Parameters parameters;
        Persistence persistence;
    public:
        // Will defer the execute until a certain point in the frame (probably a bit later)
        void ExecuteDeferred();
        // Will immediately execute the script
        void ExecuteImmediately();
        [[nodiscard]] Arca::Index<RunningScript> RunningForThis() const;
        // Returns if this script is currently running
        // NOTE: Suspension has no effect on this
        [[nodiscard]] bool IsRunning() const;
    private:
        Arca::Index<RunningScript> CreateRunningFromThis();
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Script::Instance>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Script::Instance";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::Instance, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Script::Instance, BinaryArchive>
    {};
}