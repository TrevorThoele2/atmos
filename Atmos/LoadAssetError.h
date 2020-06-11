#pragma once

#include "Exception.h"

namespace Atmos::Asset
{
    class LoadError final : public Exception
    {
    public:
        LoadError(const String& message);
        LoadError(const String& message, const NameValuePairs& details);
    };
}