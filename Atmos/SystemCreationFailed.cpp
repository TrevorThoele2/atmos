#include "SystemCreationFailed.h"

namespace Atmos::System
{
    SystemCreationFailed::SystemCreationFailed() :
        Exception(Message())
    {}

    SystemCreationFailed::SystemCreationFailed(const String& reason) :
        Exception(Message(), { { "Reason", reason } })
    {}

    String SystemCreationFailed::Message()
    {
        return "System creation failed.";
    }
}