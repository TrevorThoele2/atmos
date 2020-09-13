#include "WindowCreationFailed.h"

namespace Atmos::Window
{
    WindowCreationFailed::WindowCreationFailed() :
        Exception(Message())
    {}

    WindowCreationFailed::WindowCreationFailed(const String& reason) :
        Exception(Message(), { { "Reason", reason } })
    {}

    String WindowCreationFailed::Message()
    {
        return "Window creation failed.";
    }
}