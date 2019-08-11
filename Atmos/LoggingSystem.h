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

        void Log(const String& string, Type type);
        void Log(const String& string, Type type, NameValueVector& nvps);

        String CurrentTimeStamp();
        void ClearFile();
    protected:
        void InitializeImpl();
    private:
        void DoLog(const String& message, Type type, Optional<NameValueVector> nvps = Optional<NameValueVector>());
        void OnExit();
        File::Path OutputFilePath() const;
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
    template<>
    class Scribe<::Atmos::LoggingSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::LoggingSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}