
#include "Logger.h"
#include <Inscription\TextFile.h>
#include <ctime>
#include "Environment.h"
#include "StringUtility.h"

namespace Atmos
{
    void DoLog(const String &message, Logger::Type type, Logger::NameValueVector *nvps = nullptr)
    {
        if (message.empty())
            return;
        
        String output;
        {
            if (IsAllWhitespace(message))
                return;
        }

        ::Inscription::TextOutFile outFile(Environment::GetFileSystem()->GetExePath().Append("errorLog.txt").GetValue(), true);

        // Output time and date
        output = Logger::GetTimeValue() + ' ';

        // Output severity
        {
            switch (type)
            {
            case Logger::Type::ERROR_SEVERE:
                output.append("<SEVERE>");
                break;
            case Logger::Type::ERROR_MODERATE:
                output.append("<MODERATE>");
                break;
            case Logger::Type::ERROR_LOW:
                output.append("<LOW>");
                break;
            case Logger::Type::WARNING:
                output.append("<WARNING>");
                break;
            case Logger::Type::INFORMATION:
                output.append("<INFO>");
                break;
            }
        }

        output.append(' ' + message);
        if (message.find_last_of('\n') != message.size() - 1)
            output.append(1, '\n');

        // Output nvps
        {
            if (nvps)
            {
                for (auto &loop : *nvps)
                    output.append("        " + loop.name + ": " + ToString(loop.value) + '\n');
            }
        }

        outFile << output;
        Logger::Instance().onLogged(output);
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
        ::Inscription::TextOutFile outFile(Environment::GetFileSystem()->GetExePath().Append("errorLog.txt").GetValue());
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