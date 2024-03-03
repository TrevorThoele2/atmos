#include "FileLoggingSink.h"

#include <Inscription/OutputTextFile.h>

namespace Atmos::Logging
{
    FileSink::FileSink()
    {
        ClearFile();
    }

    void FileSink::Log(const Logging::Log& log)
    {
        const auto output = StandardFormattedLog(log);

        {
            Inscription::File::OutputText outputFile(OutputFilePath(), true);
            outputFile.WriteData(output);
        }
    }

    void FileSink::ClearFile()
    {
        Inscription::File::OutputText outFile(OutputFilePath());
    }

    File::Path FileSink::OutputFilePath()
    {
        return std::filesystem::current_path() / "log.txt";
    }
}