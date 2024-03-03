#include "LoggingCurator.h"
#include "Logger.h"

#include <Arca/Reliquary.h>

namespace Atmos::Logging
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    void Curator::Handle(const Log& log)
    {
        auto processedLog = logger.Log(log);
        if (processedLog)
            Owner().Raise(*processedLog);
    }
}