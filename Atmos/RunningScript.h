#pragma once

#include <memory>

#include <Arca/ClosedTypedRelic.h>

#include "ScriptInstance.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

class asIScriptContext;

namespace Atmos::Script
{
    class RunningScript final : public Arca::ClosedTypedRelic<RunningScript>
    {
    public:
        using Source = ScriptInstance;
        Arca::RelicIndex<Source> source;
    public:
        Name executeName;
        Parameters parameters;
        Persistence persistence;
    public:
        bool hasBeenExecuted = false;
        bool executedThisFrame = false;
    public:
        explicit RunningScript(Init init);
        RunningScript(
            Init init,
            Arca::RelicIndex<Source> source,
            const Name& executeName,
            const Parameters& parameters,
            const Persistence& persistence);
        RunningScript(const RunningScript& arg);
        ~RunningScript();

        RunningScript& operator=(const RunningScript& arg);

        void Resume();
        void Suspend();
        [[nodiscard]] bool IsSuspended() const;

        [[nodiscard]] bool ShouldExecuteMain() const;

        asIScriptContext* UnderlyingContext();
    private:
        bool isSuspended = false;
    private:
        class Data;
        std::unique_ptr<Data> data;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Script::RunningScript>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "RunningScript";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Script::RunningScript, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Script::RunningScript, BinaryArchive>
    {};
}