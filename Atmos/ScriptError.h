#pragma once

#include "Exception.h"

namespace Atmos::Scripting
{
    class Error final : public Exception
    {
    public:
        Error(const String& message);
        Error(const String& message, const NameValuePairs& details);
    };
}