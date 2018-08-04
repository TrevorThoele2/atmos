#pragma once

#include <memory>
#include "FilePath.h"

namespace Atmos
{
    namespace FileSystem
    {
        class Handler
        {
        public:
            virtual ~Handler() = 0 {}
            void Init();
            virtual FilePath GetExePath() const = 0;
            virtual FilePath GetDataPath() const = 0;
            virtual FilePath GetTempDirectoryPath() const = 0;
            virtual size_t GetMaxPathLength() const = 0;
            virtual String GetFileSeparator() const = 0;
        };
    }
}