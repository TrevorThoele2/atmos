#include "Logger.h"

#include "ProcessedLog.h"

#include "FileManagerProvider.h"

#include "StringUtility.h"

#include <Inscription/OutputTextFile.h>

namespace Atmos::Logging
{
    std::optional<ProcessedLog> Logger::Log(const Logging::Log& log)
    {
        return Log(log.message, log.severity, log.details);
    }

    std::optional<ProcessedLog> Logger::Log(
        const String& message,
        Severity severity,
        std::optional<Details> details)
    {
        if (message.empty())
            return {};

        if (IsAllWhitespace(message))
            return {};

        Inscription::OutputTextFile outFile(OutputFilePath(), true);

        // Output time and date
        auto output = CurrentTimeStamp() + ' ';

        // Output severity
        {
            switch (severity)
            {
            case Severity::Verbose:
                output.append("<VERBOSE>");
                break;
            case Severity::Information:
                output.append("<INFO>");
                break;
            case Severity::Warning:
                output.append("<WARNING>");
                break;
            case Severity::Error:
                output.append("<ERROR>");
                break;
            }
        }

        output.append(' ' + message);
        if (output.find_last_of('\n') != output.size() - 1)
            output.append(1, '\n');

        if (details)
            for (auto& loop : *details)
                output.append("        " + loop.name + ": " + ToString(loop.value) + '\n');

        outFile.WriteData(output);
        ProcessedLog processed{ output, message, severity, details };
        onLog(processed);
        return processed;
    }

    void Logger::StartSession()
    {
        ClearFile();
        Log("Session started.");
    }

    void Logger::StopSession()
    {
        Log("Session stopped.");
    }

    void Logger::ClearFile()
    {
        Inscription::OutputTextFile outFile(OutputFilePath());
    }

    File::Path Logger::OutputFilePath()
    {
        return std::filesystem::current_path() / "log.txt";
    }

    String Logger::CurrentTimeStamp()
    {
        String message;

        auto t = time(nullptr);
        struct tm now;
        localtime_s(&now, &t);

        const auto appendToMessage = [&message](auto append)
        {
            std::ostringstream stream;
            stream << std::setfill('0') << std::setw(2) << append;
            message.append(stream.str());
        };

        message.append("<");
        appendToMessage(now.tm_mday);
        message.append("/");
        appendToMessage(now.tm_mon + 1);
        message.append("/");
        appendToMessage(now.tm_year + 1900);
        message.append("> (");
        appendToMessage(now.tm_hour);
        message.append(":");
        appendToMessage(now.tm_min);
        message.append(":");
        appendToMessage(now.tm_sec);
        message.append(")");

        return message;
    }
}