
#include "Optional.h"

namespace Atmos
{
    OptionalException::OptionalException() : std::runtime_error("An invalid optional value was dereferenced.")
    {}
}