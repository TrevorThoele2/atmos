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
            FilePath GetExePath() const override final;
            FilePath GetDataPath() const override final;
            FilePath GetTempDirectoryPath() const override final;
            size_t GetMaxPathLength() const override final;
            String GetFileSeparator() const override final;
        };
    }
}