#include "LoggingCurator.h"

#include "LoggingInformation.h"

#include <Arca/Reliquary.h>

namespace Atmos::Logging
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    void Curator::Handle(const Log& log)
    {
        Arca::Index<Information>(Owner())->logger->Log(log);
    }
}