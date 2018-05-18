
#include "Error.h"
#include <Inscription\TextFile.h>
#include <ctime>
#include "Environment.h"
#include "StringUtility.h"

namespace Atmos
{
    void DoLog(const String &string, ErrorHandler::Severity severity, ErrorHandler::NameValueVector *nvps = nullptr)
    {
        if (string.empty())
            return;
        
        String message;
        // Check if the whole thing is spaces or new lines
        {
            bool leave = true;
            auto loop = string.begin();
            for (auto &loop : string)
            {
                if (loop == '\n' || loop == ' ')
                    continue;

                leave = false;
                break;
            }

            if (leave)
                return;
        }

        inscription::TextOutFile outFile(Environment::GetFileSystem()->GetExePath().Append("errorLog.txt").GetValue(), true);

        // Output time and date
        message = ErrorHandler::GetTimeValue() + "\n";

        // Output severity
        {
            switch (severity)
            {
            case ErrorHandler::Severity::ERROR_SEVERE:
                message.append("Severity: Severe Error\n");
                break;
            case ErrorHandler::Severity::ERROR_MODERATE:
                message.append("Severity: Moderate Error\n");
                break;
            case ErrorHandler::Severity::ERROR_LOW:
                message.append("Severity: Low Error\n");
                break;
            case ErrorHandler::Severity::WARNING:
                message.append("Severity: Warning\n");
                break;
            case ErrorHandler::Severity::INFORMATION:
                break;
            }
        }

        // Output nvps
        {
            if (nvps)
            {
                for (auto &loop : *nvps)
                    message.append(loop.name + ": " + loop.value.ToString() + "\n");
            }
        }

        auto placeNewLines = [](const String &check, String &change)
        {
            // If it already has one endline, push another at the back
            if (check.front() == '\n')
                change.push_back('\n');
            else
            {
                // Else push two to the back
                change.push_back('\n');
                change.push_back('\n');
            }
        };

        message.append(string);
        if (string.size() == 1 || string.size() == 2)
            placeNewLines(string, message);
        else
            placeNewLines(string.substr(string.size() - 2), message);

        outFile << message;
        ErrorHandler::Instance().onLogged(message);
    }

    void ErrorHandler::Log(const String &string, Severity severity)
    {
        if (!Instance().initialized)
        {
            Instance().entries.push_back(Entry(string, severity, NameValueVector()));
            return;
        }

        DoLog(string, severity);
    }

    void ErrorHandler::Log(const String &string, Severity severity, ErrorHandler::NameValueVector &nvps)
    {
        if (!Instance().initialized)
        {
            Instance().entries.push_back(Entry(string, severity, nvps));
            return;
        }

        DoLog(string, severity, &nvps);
    }

    String ErrorHandler::GetTimeValue()
    {
        String ret;

        time_t t = time(nullptr);
        struct tm now;
        localtime_s(&now, &t);

        auto widthAppender = [&](auto append)
        {
            std::ostringstream stream;
            stream << std::setfill('0') << std::setw(2) << append;
            ret.append(stream.str());
        };

        ret.append("<");
        widthAppender(now.tm_mday);
        ret.append("/");
        widthAppender(now.tm_mon + 1);
        ret.append("/");
        widthAppender(now.tm_year + 1900);
        ret.append("> (");
        widthAppender(now.tm_hour);
        ret.append(":");
        widthAppender(now.tm_min);
        ret.append(":");
        widthAppender(now.tm_sec);
        ret.append(")");

        return ret;
    }

    void ErrorHandler::ClearFile()
    {
        // Just open up the file without appending
        inscription::TextOutFile outFile(Environment::GetFileSystem()->GetExePath().Append("errorLog.txt").GetValue());
    }

    ErrorHandler::Entry::Entry(const String &string, Severity severity, const NameValueVector &nvps) : string(string), severity(severity), nvps(nvps)
    {}

    void OnExit()
    {
        DoLog("Session stop.", ErrorHandler::Severity::INFORMATION);
    }

    ErrorHandler& ErrorHandler::Instance()
    {
        static ErrorHandler instance;
        return instance;
    }

    void ErrorHandler::Init()
    {
        Instance().initialized = true;
        ClearFile();
        DoLog("Session start.", ErrorHandler::Severity::INFORMATION);
        std::atexit(&OnExit);

        // Push all stacked entries
        auto entryLoop = Instance().entries.begin();
        while (entryLoop != Instance().entries.end())
        {
            if (entryLoop->nvps.empty())
                Log(entryLoop->string, entryLoop->severity);
            else
                Log(entryLoop->string, entryLoop->severity, entryLoop->nvps);
            entryLoop = Instance().entries.erase(entryLoop);
        }
    }

    ErrorHandler::ErrorHandler() : initialized(false)
    {}
}