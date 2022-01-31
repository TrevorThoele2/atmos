#include "FileLoggingSink.h"

#include <Inscription/OutputTextFile.h>

namespace Atmos::Logging
{
    FileSink::FileSink()
    {
        Inscription::File::OutputText outputFile(OutputFilePath());
    }

    void FileSink::Log(const Logging::Log& log)
    {
        logs.push_back(log);
    }

    void FileSink::Flush()
    {
        std::vector<String> logMessages;
        logMessages.reserve(logs.size());
        for (auto& log : logs)
            logMessages.push_back(StandardFormattedLog(log));

        auto data = Chroma::Join(logMessages.begin(), logMessages.end());

        Inscription::File::OutputText outputFile(OutputFilePath());
        outputFile.WriteData(data);
    }

    File::Path FileSink::OutputFilePath()
    {
        return std::filesystem::current_path() / "log.txt";
    }
}