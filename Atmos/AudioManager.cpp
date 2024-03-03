#include "AudioManager.h"

namespace Atmos::Audio
{
    Manager::~Manager()
    {
        logger->Log(
            "Destroyed audio.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    Manager::Manager(Logging::Logger& logger, String typeName) :
        typeName(typeName), logger(&logger)
    {
        logger.Log(
            "Created audio.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }
}