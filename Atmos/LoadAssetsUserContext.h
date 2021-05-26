#pragma once

#include "Name.h"
#include "Buffer.h"
#include <optional>

namespace Inscription
{
    class LoadAssetsUserContext
    {
    public:
        struct Extracted
        {
            Atmos::Name name;
            Atmos::Buffer memory;
            Extracted() = default;
            Extracted(const Atmos::Name& name, Atmos::Buffer&& memory);
        };
    public:
        virtual ~LoadAssetsUserContext() = 0;

        virtual std::optional<Extracted> LoadAudioData(const Atmos::Name& name) = 0;
        virtual std::optional<Extracted> LoadImageData(const Atmos::Name& name) = 0;
        virtual std::optional<Extracted> LoadShaderData(const Atmos::Name& name) = 0;
        virtual std::optional<Extracted> LoadFontData(const Atmos::Name& name) = 0;
        virtual std::optional<Extracted> LoadScriptData(const Atmos::Name& name) = 0;
    };
}