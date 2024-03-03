#include "LoadAssetError.h"

namespace Atmos::Asset
{
    LoadError::LoadError(const String& message) :
        Exception(message)
    {}

    LoadError::LoadError(const String& message, const NameValuePairs& details) :
        Exception(message, details)
    {}
}