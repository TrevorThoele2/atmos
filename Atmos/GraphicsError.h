#pragma once

#include "Exception.h"

namespace Atmos::Render
{
    class GraphicsError final : public Exception
    {
    public:
        GraphicsError(const String& message);
        GraphicsError(const String& message, NameValuePairs details);
    };
}