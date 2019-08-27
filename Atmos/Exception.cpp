#include "Exception.h"

namespace Atmos
{
    Exception::Exception(const String& message) : exception(message.c_str())
    {}
}