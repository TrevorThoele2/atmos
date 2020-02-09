#include "LoggingCurator.h"
#include "Logger.h"

namespace Atmos::Logging
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    void Curator::Handle(const Log& log)
    {
        logger.Log(log);
    }
}