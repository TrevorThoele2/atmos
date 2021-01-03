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

        virtual std::optional<Extracted> LoadImage(const Atmos::Name& name) = 0;
        virtual std::optional<Extracted> LoadShader(const Atmos::Name& name) = 0;
        virtual std::optional<Extracted> LoadScript(const Atmos::Name& name) = 0;
    };
}