
#include "ObjectManagerException.h"

namespace Atmos
{
    ObjectManagerException::ObjectManagerException(const String& message) : runtime_error(message)
    {}

    IncorrectRegisteredObjectSystemTypeException::IncorrectRegisteredObjectSystemTypeException() :
        ObjectManagerException("The type given does not match the object system's runtime type.")
    {}
}