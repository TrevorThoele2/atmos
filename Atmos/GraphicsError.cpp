#include "GraphicsError.h"

namespace Atmos::Render
{
    GraphicsError::GraphicsError(const String& message) : Exception(message)
    {}

    GraphicsError::GraphicsError(const String& message, NameValuePairs details) : Exception(message, details)
    {}
}