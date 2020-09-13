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
        Arca::Index<Asset::Script> asset;
    public:
        Name executeName;
        Parameters parameters;
        Persistence persistence;
    public:
        Instance(
            Init init,
            Arca::Index<Asset::Script> asset,
            Name executeName,
            Parameters parameters,
            Persistence persistence);
        Instance(Init init, Arca::Serialization);

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
    struct Traits<Atmos::Script::Instance>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Script::Instance";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Script::Instance, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Script::Instance>;
    };
}