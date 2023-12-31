#pragma once

#include "LoggingSink.h"

#include "FilePath.h"

namespace Atmos::Logging
{
    class FileSink final : public Sink
    {
    public:
        FileSink();

        void Log(const Logging::Log& log) override;
        void Flush() override;
    private:
        std::vector<Logging::Log> logs;

        [[nodiscard]] static File::Path OutputFilePath();
    };
}