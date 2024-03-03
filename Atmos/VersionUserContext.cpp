#include "VersionUserContext.h"

namespace Atmos
{
    VersionUserContext::VersionUserContext(
        Inscription::Version inscriptionVersion, Inscription::Version clientVersion)
        :
        inscriptionVersion(inscriptionVersion), clientVersion(clientVersion)
    {}
}