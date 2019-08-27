#pragma once

#include <memory>

#include <Arca/ClosedTypedRelicAutomation.h>

#include "ScriptInstance.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

class asIScriptContext;

namespace Atmos::Script
{
    class RunningScript final : public Arca::ClosedTypedRelicAutomation<RunningScript>
    {
    public:
        using Source = ScriptInstance;
        Source* source = nullptr;
    public:
        Name executeName;
        Parameters parameters;
        Persistence persistence;
    public:
        bool hasBeenExecuted = false;
        bool executedThisFrame = false;
    public:
        RunningScript();
        RunningScript(const RunningScript& arg);
        ~RunningScript();

        RunningScript& operator=(const RunningScript& arg);

        void Resume();
        void Suspend();
        [[nodiscard]] bool IsSuspended() const;

        [[nodiscard]] bool ShouldExecuteMain() const;

        asIScriptContext* UnderlyingContext();
    public:
        void Initialize(Source& source);
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
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Script::RunningScript, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Script::RunningScript, BinaryArchive>
    {};
}