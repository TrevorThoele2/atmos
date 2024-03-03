#include "AllRunningScripts.h"

namespace Atmos::Script
{
    void AllRunningScripts::Add(ScriptInstance& instance, RunningScript& runningScript)
    {
        map.emplace(&instance, &runningScript);
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

    RunningScript* AllRunningScripts::RunningScriptFor(const ScriptInstance& instance) const
    {
        const auto found = map.find(&const_cast<ScriptInstance&>(instance));
        if (found == map.end())
            return {};

        return found->second;
    }

    bool AllRunningScripts::IsRunning(const ScriptInstance& instance) const
    {
        return static_cast<bool>(RunningScriptFor(instance));
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

namespace Arca
{
    const TypeName Traits<Atmos::Script::AllRunningScripts>::typeName = "RunningScriptInformation";
}