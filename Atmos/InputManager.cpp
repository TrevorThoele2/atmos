#include "InputManager.h"

namespace Atmos::Input
{
    Manager::Manager(Logging::Logger& logger, String typeName) :
        typeName(typeName), logger(&logger)
    {
        logger.Log(
            "Created input.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    Manager::~Manager()
    {
        logger->Log(
            "Destroyed input.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }
}