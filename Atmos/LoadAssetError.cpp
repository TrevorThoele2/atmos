#include "LoadAssetError.h"

namespace Atmos::Asset
{
    LoadAssetError::LoadAssetError(const String& message) :
        Exception(message)
    {}

    LoadAssetError::LoadAssetError(const String& message, const NameValuePairs& details) :
        Exception(message, details)
    {}
}