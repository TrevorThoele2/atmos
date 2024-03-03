#include "GlobalInitializer.h"
#include "Logger.h"

namespace Atmos
{
    GlobalInitializer::GlobalInitializer()
    {
        if (!isInitialized)
        {
            _CrtSetDbgFlag(0);
            FreeImage_Initialise();
            FreeImage_SetOutputMessage(ErrorHandler);
            isInitialized = false;
        }
    }

    GlobalInitializer::~GlobalInitializer()
    {
        if (isInitialized)
        {
            FreeImage_DeInitialise();
            isInitialized = false;
        }
    }

    void GlobalInitializer::ErrorHandler(FREE_IMAGE_FORMAT format, const char* message)
    {
        const auto retrievedFormatMessage = FreeImage_GetFormatFromFIF(format);
        const String formatMessage = retrievedFormatMessage
            ? retrievedFormatMessage
            : "Unknown";

        Logging::logger.Log(
            message,
            Logging::Severity::Error,
            Logging::Details{ { "Format", formatMessage } });
    }
}
