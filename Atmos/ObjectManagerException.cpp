
#include "ObjectManagerException.h"

namespace Atmos
{
    ObjectManagerException::ObjectManagerException(const String& message) : runtime_error(message)
    {}

    IncorrectRegisteredObjectSystemTypeException::IncorrectRegisteredObjectSystemTypeException() :
        ObjectManagerException("The type given does not match the object system's runtime type.")
    {}

    ObjectFactoryDoesntExist::ObjectFactoryDoesntExist() :
        ObjectManagerException("There is no object factory for this type. Perhaps missing a registration?")
    {}
}