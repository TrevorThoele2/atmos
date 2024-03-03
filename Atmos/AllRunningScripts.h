#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "RunningScript.h"
#include <unordered_map>

namespace Atmos::Script
{
    class AllRunningScripts final : public Arca::ClosedTypedRelic<AllRunningScripts>
    {
    private:
        using Map = std::unordered_map<Arca::RelicID, Arca::RelicIndex<RunningScript>>;
    public:
        using iterator = Map::iterator;
        using const_iterator = Map::const_iterator;
    public:
        void Add(Arca::RelicID instanceID, Arca::RelicIndex<RunningScript> runningScript);
        void Remove(const_iterator script);

        [[nodiscard]] const_iterator CurrentlyRunning() const;

        [[nodiscard]] Arca::RelicIndex<RunningScript> RunningScriptFor(Arca::RelicID instanceID) const;
        [[nodiscard]] bool IsRunning(Arca::RelicID instanceID) const;
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
        static inline const TypeName typeName = "RunningScriptInformation";
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