#include "ScriptError.h"

namespace Atmos::Scripting
{
    Error::Error(const String& message) :
        Exception(message)
    {}

    Error::Error(const String& message, const NameValuePairs& details) :
        Exception(message, details)
    {}
}