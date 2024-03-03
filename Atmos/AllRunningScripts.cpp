#include "AllRunningScripts.h"

namespace Atmos::Script
{
    void AllRunningScripts::Add(Arca::RelicID instanceID, Arca::RelicIndex<RunningScript> runningScript)
    {
        map.emplace(instanceID, runningScript);
    }

    void AllRunningScripts::Remove(const_iterator script)
    {
        script == currentRunning
            ? currentRunning = map.erase(script)
            : map.erase(script);
    }

    auto AllRunningScripts::CurrentlyRunning() const -> const_iterator
    {
        if (currentRunning == map.end())
            return {};

        return currentRunning;
    }

    Arca::RelicIndex<RunningScript> AllRunningScripts::RunningScriptFor(Arca::RelicID instanceID) const
    {
        const auto found = map.find(instanceID);
        if (found == map.end())
            return {};

        return found->second;
    }

    bool AllRunningScripts::IsRunning(Arca::RelicID instanceID) const
    {
        return static_cast<bool>(RunningScriptFor(instanceID));
    }

    size_t AllRunningScripts::Size() const
    {
        return map.size();
    }

    auto AllRunningScripts::begin() -> iterator
    {
        return map.begin();
    }

    auto AllRunningScripts::end() -> iterator
    {
        return map.end();
    }

    auto AllRunningScripts::begin() const -> const_iterator
    {
        return map.begin();
    }

    auto AllRunningScripts::end() const -> const_iterator
    {
        return map.end();
    }
}