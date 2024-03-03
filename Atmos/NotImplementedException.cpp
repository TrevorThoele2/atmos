
#include "NotImplementedException.h"

namespace Atmos
{
    NotImplementedException::NotImplementedException(const String& message) : runtime_error(message.c_str())
    {}
}