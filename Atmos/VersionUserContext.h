#pragma once

#include <Inscription/Version.h>

namespace Atmos
{
    struct VersionUserContext
    {
        Inscription::Version inscriptionVersion = 0;
        Inscription::Version clientVersion = 0;

        VersionUserContext() = default;
        VersionUserContext(Inscription::Version inscriptionVersion, Inscription::Version clientVersion);
    };
}