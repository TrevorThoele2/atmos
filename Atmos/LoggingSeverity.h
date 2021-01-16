#pragma once

#include "StringUtility.h"

namespace Atmos::Logging
{
    enum class Severity
    {
        Verbose,
        Information,
        Warning,
        Error
    };
}

namespace Atmos
{
    String ToString(Logging::Severity severity);
}