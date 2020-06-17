#pragma once

namespace Atmos::Asset::Resource
{
    class Shader
    {
    public:
        virtual ~Shader() = 0;
    protected:
        Shader() = default;
    };
}