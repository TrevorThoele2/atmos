#pragma once

#include "FontAssetResource.h"

namespace Atmos::Render
{
    struct FontID
    {
        const Asset::Resource::Font* resource;
        bool bold;
        bool italics;

        FontID(const Asset::Resource::Font* resource, bool bold, bool italics);
        FontID(const FontID& arg) = default;
        bool operator==(const FontID& arg) const;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::Render::FontID>
    {
        using argument_type = Atmos::Render::FontID;
        using result_type = std::size_t;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const auto first(std::hash<const Atmos::Asset::Resource::Font*>()(arg.resource));
            const auto second(std::hash<bool>()(arg.bold));
            const auto third(std::hash<bool>()(arg.italics));
            return first ^ (second << 1) ^ (third >> 1);
        }
    };
}