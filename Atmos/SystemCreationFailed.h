#pragma once

#include "Exception.h"

namespace Atmos::System
{
    class SystemCreationFailed final : public Exception
    {
    public:
        SystemCreationFailed();
        SystemCreationFailed(const String& reason);
    private:
        static String Message();
    };
}