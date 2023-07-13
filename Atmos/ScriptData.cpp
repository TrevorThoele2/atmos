#include "ScriptData.h"

namespace Atmos::Scripting
{
    ScriptData::ScriptData(const Arca::Index<Asset::Script>& asset, const String& executeName, const Parameters& parameters) :
        asset(asset), executeName(executeName), parameters(parameters)
    {}

    ScriptData::ScriptData(ScriptData&& arg) noexcept :
        asset(std::move(arg.asset)),
        executeName(std::move(arg.executeName)),
        parameters(std::move(arg.parameters)),
        isSuspended(arg.isSuspended)
    {}

    ScriptData& ScriptData::operator=(ScriptData&& arg) noexcept
    {
        asset = std::move(arg.asset);
        executeName = std::move(arg.executeName);
        parameters = std::move(arg.parameters);
        isSuspended = arg.isSuspended;
        return *this;
    }

    bool ScriptData::operator==(const ScriptData& arg) const
    {
        return IndexEqualsValue(asset, arg.asset)
            && executeName == arg.executeName
            && parameters == arg.parameters
            && isSuspended == arg.isSuspended;
    }

    bool ScriptData::operator!=(const ScriptData& arg) const
    {
        return !(*this == arg);
    }
}