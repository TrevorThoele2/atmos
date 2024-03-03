#pragma once

#include "Exception.h"

namespace Atmos::Audio
{
    class Error final : public Exception
    {
    public:
        Error(const String& message);
        Error(const String& message, NameValuePairs details);
    };
}