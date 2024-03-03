#pragma once

#include "TypeRegistration.h"

namespace Atmos
{
    void RegisterGlobalTypes(TypeRegistration::Group& to);
    void RegisterLocalTypes(TypeRegistration::Group& to);
    void RegisterInfrastructureTypes(TypeRegistration::Group& to);
}