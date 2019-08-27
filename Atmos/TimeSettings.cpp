#include "TimeSettings.h"

namespace Atmos::Time
{
    const Fps Settings::defaultFpsLimit = 60;
}

namespace Arca
{
    const TypeName Traits<::Atmos::Time::Settings>::typeName = "TimeSettings";
}