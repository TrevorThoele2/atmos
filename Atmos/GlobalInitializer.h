#pragma once

#include "FreeImageIncludes.h"

namespace Atmos
{
    class GlobalInitializer
    {
    public:
        GlobalInitializer();
        ~GlobalInitializer();
    private:
        bool isInitialized = false;

        static void ErrorHandler(FREE_IMAGE_FORMAT format, const char* message);
    };

    inline GlobalInitializer globalInitializer;
}