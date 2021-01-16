#include "AudioError.h"

namespace Atmos::Audio
{
    Error::Error(const String& message) : Exception(message)
    {}

    Error::Error(const String& message, NameValuePairs details) : Exception(message, details)
    {}
}