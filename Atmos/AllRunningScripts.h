#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "RunningScript.h"
#include <unordered_map>

namespace Atmos::Script
{
    class AllRunningScripts final : public Arca::ClosedTypedRelicAutomation<AllRunningScripts>
    {
    private:
        using Map = std::unordered_map<ScriptInstance*, RunningScript*>;
    public:
        using iterator = Map::iterator;
        using const_iterator = Map::const_iterator;
    public:
        void Add(ScriptInstance& instance, RunningScript& runningScript);
        void Remove(const_iterator script);

        [[nodiscard]] const_iterator CurrentlyRunning() const;

        [[nodiscard]] RunningScript* RunningScriptFor(const ScriptInstance& instance) const;
        [[nodiscard]] bool IsRunning(const ScriptInstance& instance) const;
        [[nodiscard]] size_t Size() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] const_iterator end() const;
    private:
        Map map;

        Map::iterator currentRunning;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::AllRunningScripts>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Script::AllRunningScripts, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Script::AllRunningScripts, BinaryArchive>
    {};
}