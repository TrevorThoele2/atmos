
#include "ObjectException.h"

namespace Atmos
{
    ObjectException::ObjectException(const String& message) : runtime_error(message.c_str())
    {}

    ObjectNonexistantException::ObjectNonexistantException() : ObjectException("An object was requested but does not exist.")
    {}
}