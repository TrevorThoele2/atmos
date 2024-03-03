#pragma once

#include "Exception.h"

namespace Atmos::Asset
{
    class LoadAssetError final : public Exception
    {
    public:
        LoadAssetError(const String& message);
        LoadAssetError(const String& message, const NameValuePairs& details);
    };
}