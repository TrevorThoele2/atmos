#pragma once

#include "FileSystem.h"
#include "WindowsInclude.h"

namespace Atmos
{
    namespace FileSystem
    {
        class WindowsHandler : public Handler
        {
        private:
            static constexpr size_t maxPathLength = MAX_PATH * sizeof(TCHAR) + sizeof(TCHAR);
        public:
            FilePath GetExePath() const override;
            FilePath GetDataPath() const override;
            FilePath GetTempDirectoryPath() const override;
            size_t GetMaxPathLength() const override;
        };
    }
}