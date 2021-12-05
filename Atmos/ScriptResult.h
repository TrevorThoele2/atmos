#pragma once

#include <optional>
#include "ScriptVariant.h"
#include "Log.h"

namespace Atmos::Scripting
{
    struct Failure
    {
        String message;

        std::optional<String> function;

        String line;
        String column;
        std::optional<String> section;
    };

    struct Quit
    {};

    using Result = std::variant<Variant, Failure, Quit>;

    Logging::Log ToLog(Failure failure);
}