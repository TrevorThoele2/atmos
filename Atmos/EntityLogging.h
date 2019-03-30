#pragma once

#include <vector>

#include "Logger.h"
#include "String.h"

namespace Atmos
{
    namespace Logging
    {
        void EntityNeedsRequiredComponents(const String &message, Logger::Type type, const std::vector<String> needComponentNames, const std::vector<String> hasComponentNames, const Logger::NameValueVector &additionals);
    }
}