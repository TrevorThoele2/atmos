#include "LoggingInformation.h"

namespace Atmos::Logging
{
    Information::Information(Logger& logger) : logger(&logger)
    {}
}