#include "ScriptManager.h"

namespace Atmos::Scripting
{
    Manager::~Manager()
    {
        logger->Log(
            "Destroyed scripts.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    Manager::Manager(Logging::Logger& logger, String typeName) :
        typeName(typeName), logger(&logger)
    {
        logger.Log(
            "Created scripts.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }
}