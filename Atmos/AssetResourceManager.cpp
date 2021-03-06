#include "AssetResourceManager.h"

namespace Atmos::Asset::Resource
{
    Manager::~Manager()
    {
        logger->Log(
            "Destroyed asset resource.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    Manager::Manager(Logging::Logger& logger, String typeName) :
        typeName(typeName), logger(&logger)
    {
        logger.Log(
            "Created asset resource.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }
}