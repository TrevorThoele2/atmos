#pragma once

namespace Atmos::Asset
{
    class ScriptData
    {
    public:
        virtual ~ScriptData() = 0;
    protected:
        ScriptData() = default;
    };
}