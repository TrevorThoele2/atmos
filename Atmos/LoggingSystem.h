#pragma once

#include <vector>

#include "ObjectSystem.h"

#include "FilePath.h"
#include "Optional.h"
#include "NameValuePair.h"
#include "Event.h"

namespace Atmos
{
    enum class LogType
    {
        INFORMATION,
        WARNING,
        ERROR_LOW,
        ERROR_SEVERE
    };

    typedef std::vector<NameValuePair> LogNameValueVector;

    class LoggingSystem : public ObjectSystem
    {
    public:
        typedef LogType Type;
        typedef LogNameValueVector NameValueVector;
    public:
        Event<const String&> onLogged;
    public:
        LoggingSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(LoggingSystem);

        void Log(const String& string, Type type);
        void Log(const String& string, Type type, NameValueVector& nvps);

        String CurrentTimeStamp();
        void ClearFile();
    private:
        void InitializeImpl();
    private:
        void DoLog(const String& message, Type type, Optional<NameValueVector> nvps = Optional<NameValueVector>());
        void OnExit();
        FilePath OutputFilePath() const;
    private:
        struct Entry
        {
            String string;
            Type type;
            NameValueVector nvps;

            Entry(const String& string, Type type, const NameValueVector& nvps);
        };

        std::vector<Entry> queuedEntryList;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::LoggingSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}