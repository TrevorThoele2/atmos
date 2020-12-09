#include "Logger.h"

#include "ProcessedLog.h"

#include "FileManagerProvider.h"

#include "StringUtility.h"

#include <Inscription/OutputTextFile.h>

namespace Atmos::Logging
{
    Logger::Logger(Severity minimumSeverity) : minimumSeverity(minimumSeverity)
    {
        StartSession();
    }

    Logger::~Logger()
    {
        StopSession();
    }

    std::optional<ProcessedLog> Logger::Log(const Logging::Log& log)
    {
        return Log(log.message, log.severity, log.details);
    }

    std::optional<ProcessedLog> Logger::Log(
        const String& message,
        Severity severity,
        std::optional<Details> details)
    {
        if (static_cast<int>(severity) < static_cast<int>(minimumSeverity))
            return {};

        const auto output =
            CurrentTimeStamp() +
            ' ' +
            SeverityToString(severity) +
            ' ' +
            message +
            (message.find_last_of('\n') != message.size() - 1 ? "\n" : "") +
            DetailsToString(details);

        {
            Inscription::OutputTextFile outputFile(OutputFilePath(), true);
            outputFile.WriteData(output);
        }

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

    String Logger::SeverityToString(Severity severity)
    {
        switch (severity)
        {
        case Severity::Verbose:
            return "<VERBOSE>";
        case Severity::Information:
            return "<INFO>";
        case Severity::Warning:
            return "<WARNING>";
        case Severity::Error:
            return "<ERROR>";
        }

        throw std::runtime_error("Logger does not understand severity type.");
    }

    String Logger::DetailsToString(std::optional<Details> details)
    {
        String output;
        if (details)
            for (auto& loop : *details)
                output.append("        " + loop.name + ": " + ToString(loop.value) + '\n');

        return output;
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