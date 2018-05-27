
#include "Logger.h"
#include <Inscription\TextFile.h>
#include <ctime>
#include "Environment.h"
#include "StringUtility.h"

namespace Atmos
{
    void DoLog(const String &string, Logger::Type type, Logger::NameValueVector *nvps = nullptr)
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
        message = Logger::GetTimeValue() + "\n";

        // Output severity
        {
            switch (type)
            {
            case Logger::Type::ERROR_SEVERE:
                message.append("Type: Severe Error\n");
                break;
            case Logger::Type::ERROR_MODERATE:
                message.append("Type: Moderate Error\n");
                break;
            case Logger::Type::ERROR_LOW:
                message.append("Type: Low Error\n");
                break;
            case Logger::Type::WARNING:
                message.append("Type: Warning\n");
                break;
            case Logger::Type::INFORMATION:
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
        Logger::Instance().onLogged(message);
    }

    void Logger::Log(const String &string, Type type)
    {
        if (!Instance().initialized)
        {
            Instance().entries.push_back(Entry(string, type, NameValueVector()));
            return;
        }

        DoLog(string, type);
    }

    void Logger::Log(const String &string, Type type, Logger::NameValueVector &nvps)
    {
        if (!Instance().initialized)
        {
            Instance().entries.push_back(Entry(string, type, nvps));
            return;
        }

        DoLog(string, type, &nvps);
    }

    String Logger::GetTimeValue()
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

    void Logger::ClearFile()
    {
        // Just open up the file without appending
        inscription::TextOutFile outFile(Environment::GetFileSystem()->GetExePath().Append("errorLog.txt").GetValue());
    }

    Logger::Entry::Entry(const String &string, Type type, const NameValueVector &nvps) : string(string), type(type), nvps(nvps)
    {}

    void OnExit()
    {
        DoLog("Session stop.", Logger::Type::INFORMATION);
    }

    Logger& Logger::Instance()
    {
        static Logger instance;
        return instance;
    }

    void Logger::Init()
    {
        Instance().initialized = true;
        ClearFile();
        DoLog("Session start.", Logger::Type::INFORMATION);
        std::atexit(&OnExit);

        // Push all stacked entries
        auto entryLoop = Instance().entries.begin();
        while (entryLoop != Instance().entries.end())
        {
            if (entryLoop->nvps.empty())
                Log(entryLoop->string, entryLoop->type);
            else
                Log(entryLoop->string, entryLoop->type, entryLoop->nvps);
            entryLoop = Instance().entries.erase(entryLoop);
        }
    }

    Logger::Logger() : initialized(false)
    {}
}