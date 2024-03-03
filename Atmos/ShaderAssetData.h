#pragma once

namespace Atmos::Asset
{
    class ShaderData
    {
    public:
        virtual ~ShaderData() = 0;
    protected:
        ShaderData() = default;
    };
}