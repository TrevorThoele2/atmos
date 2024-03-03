
#include "ObjectException.h"

namespace Atmos
{
    ObjectException::ObjectException(const String& message) : runtime_error(message)
    {}
}