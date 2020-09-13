#pragma once

#include "Exception.h"

namespace Atmos::Window
{
    class WindowCreationFailed final : public Exception
    {
    public:
        WindowCreationFailed();
        WindowCreationFailed(const String& reason);
    private:
        static String Message();
    };
}