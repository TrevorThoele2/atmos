
#include <ctime>

#include "LoggingSystem.h"

#include "ObjectManager.h"
#include "FileSystem.h"

#include "StringUtility.h"

#include <Inscription\TextFile.h>

namespace Atmos
{
    LoggingSystem::LoggingSystem(ObjectManager& manager) :
        ObjectSystem(manager)
    {}

    LoggingSystem::LoggingSystem(const ::Inscription::Table<LoggingSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void LoggingSystem::Log(const String& string, Type type)
    {
        if (!IsInitialized())
        {
            queuedEntryList.push_back(Entry(string, type, NameValueVector()));
            return;
        }

        DoLog(string, type);
    }

    void LoggingSystem::Log(const String& string, Type type, LoggingSystem::NameValueVector& nvps)
    {
        if (!IsInitialized())
        {
            queuedEntryList.push_back(Entry(string, type, nvps));
            return;
        }

        DoLog(string, type, nvps);
    }

    String LoggingSystem::TimeValue()
    {
        String timeValue;

        time_t t = time(nullptr);
        struct tm now;
        localtime_s(&now, &t);

        auto widthAppender = [&](auto append)
        {
            std::ostringstream stream;
            stream << std::setfill('0') << std::setw(2) << append;
            timeValue.append(stream.str());
        };

        timeValue.append("<");
        widthAppender(now.tm_mday);
        timeValue.append("/");
        widthAppender(now.tm_mon + 1);
        timeValue.append("/");
        widthAppender(now.tm_year + 1900);
        timeValue.append("> (");
        widthAppender(now.tm_hour);
        timeValue.append(":");
        widthAppender(now.tm_min);
        timeValue.append(":");
        widthAppender(now.tm_sec);
        timeValue.append(")");

        return timeValue;
    }

    void LoggingSystem::ClearFile()
    {
        // Just open up the file without appending
        ::Inscription::TextOutFile outFile(OutputFilePath());
    }

    void LoggingSystem::InitializeImpl()
    {
        ClearFile();
        DoLog("Session start.", LoggingSystem::Type::INFORMATION);

        auto entryLoop = queuedEntryList.begin();
        while (entryLoop != queuedEntryList.end())
        {
            if (entryLoop->nvps.empty())
                Log(entryLoop->string, entryLoop->type);
            else
                Log(entryLoop->string, entryLoop->type, entryLoop->nvps);
            entryLoop = queuedEntryList.erase(entryLoop);
        }
    }

    void LoggingSystem::DoLog(const String& message, LoggingSystem::Type type, Optional<LoggingSystem::NameValueVector> nvps)
    {
        if (message.empty())
            return;

        if (IsAllWhitespace(message))
            return;

        String output;

        ::Inscription::TextOutFile outFile(OutputFilePath(), true);

        // Output time and date
        output = TimeValue() + ' ';

        // Output severity
        {
            switch (type)
            {
            case LoggingSystem::Type::ERROR_SEVERE:
                output.append("<SEVERE>");
                break;
            case LoggingSystem::Type::ERROR_LOW:
                output.append("<LOW>");
                break;
            case LoggingSystem::Type::WARNING:
                output.append("<WARNING>");
                break;
            case LoggingSystem::Type::INFORMATION:
                output.append("<INFO>");
                break;
            }
        }

        output.append(' ' + message);
        if (output.find_last_of('\n') != output.size() - 1)
            output.append(1, '\n');

        // Output nvps
        {
            if (nvps)
            {
                for (auto& loop : *nvps)
                    output.append("        " + loop.name + ": " + ToString(loop.value) + '\n');
            }
        }

        outFile << output;
        onLogged(output);
    }

    void LoggingSystem::OnExit()
    {
        DoLog("Session stop.", LoggingSystem::Type::INFORMATION);
    }

    FilePath LoggingSystem::OutputFilePath() const
    {
        const FileName fileName("log.txt");
        auto fileManager = Manager()->FindSystem<FileSystem>()->Get();
        return fileManager->ExePath().SetFileName(fileName).GetValue();
    }

    LoggingSystem::Entry::Entry(const String& string, Type type, const NameValueVector& nvps) : string(string), type(type), nvps(nvps)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::LoggingSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::LoggingSystem, "LoggingSystem");
}