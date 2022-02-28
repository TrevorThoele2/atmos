#pragma once

#include "ShaderAsset.h"

namespace Atmos::Render
{
    struct Shaders
    {
        const Asset::Shader* vertex = nullptr;
        const Asset::Shader* fragment = nullptr;

        bool operator==(const Shaders& arg) const;
        bool operator!=(const Shaders& arg) const;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::Render::Shaders>
    {
        using argument_type = Atmos::Render::Shaders;
        using result_type = std::size_t;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const result_type first(std::hash<const Atmos::Asset::Shader*>()(arg.vertex));
            const result_type second(std::hash<const Atmos::Asset::Shader*>()(arg.fragment));
            return first ^ (second << 1);
        }
    };
}