#pragma once

#include <Arca/Command.h>

#include "RasterCommand.h"

namespace Atmos::Render::Raster
{
    struct RecordCommands
    {
        std::vector<Command> commands = {};
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Raster::RecordCommands>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::Raster::RecordCommands";
    };
}