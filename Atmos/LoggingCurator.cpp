#include <ctime>

#include "LoggingCurator.h"

#include "ProcessedLog.h"

#include <Arca/Reliquary.h>
#include "FileManagerProvider.h"

#include "FileName.h"
#include "StringUtility.h"

#include <Inscription/OutputTextFile.h>

namespace Atmos::Logging
{
    String Curator::CurrentTimeStamp()
    {
        String message;

        auto t = time(nullptr);
        struct tm now;
        localtime_s(&now, &t);

        const auto widthAppender = [&message](auto append)
        {
            std::ostringstream stream;
            stream << std::setfill('0') << std::setw(2) << append;
            message.append(stream.str());
        };

        message.append("<");
        widthAppender(now.tm_mday);
        message.append("/");
        widthAppender(now.tm_mon + 1);
        message.append("/");
        widthAppender(now.tm_year + 1900);
        message.append("> (");
        widthAppender(now.tm_hour);
        message.append(":");
        widthAppender(now.tm_min);
        message.append(":");
        widthAppender(now.tm_sec);
        message.append(")");

        return message;
    }

    void Curator::ClearFile()
    {
        // Just open up the file without appending
        ::Inscription::OutputTextFile outFile(OutputFilePath());
    }

    void Curator::InitializeImplementation()
    {
        ClearFile();
        DoLog("Session start.", Severity::Information);

        Owner().ExecuteOn<Log>(
            [this](const Log& log)
            {
                DoLog(log.message, log.severity, log.details);
            });
    }

    void Curator::DoLog(const String& message, Severity severity, std::optional<Details> details)
    {
        if (message.empty())
            return;

        if (IsAllWhitespace(message))
            return;

        ::Inscription::OutputTextFile outFile(OutputFilePath(), true);

        // Output time and date
        auto output = CurrentTimeStamp() + ' ';

        // Output severity
        {
            switch (severity)
            {
            case Severity::SevereError:
                output.append("<SEVERE>");
                break;
            case Severity::LowError:
                output.append("<LOW>");
                break;
            case Severity::Warning:
                output.append("<WARNING>");
                break;
            case Severity::Information:
                output.append("<INFO>");
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
        Owner().Raise<ProcessedLog>(output, message, severity, details);
    }

    void Curator::OnExit()
    {
        DoLog("Session stop.", Severity::Information);
    }

    File::Path Curator::OutputFilePath()
    {
        const File::Name fileName("log.txt");
        return File::manager->ExePath().SetFileName(fileName).GetValue();
    }
}