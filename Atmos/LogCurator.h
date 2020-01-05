#pragma once

#include <Arca/Curator.h>
#include <Arca/SignalBatch.h>

#include "Log.h"
#include "LogSeverity.h"
#include "LogNameValuePairs.h"

#include "FilePath.h"

namespace Atmos::Logging
{
    class Curator final : public Arca::Curator
    {
    public:
        String CurrentTimeStamp();
        void ClearFile();
    protected:
        void InitializeImplementation() override;
    private:
        Arca::Batch<Log> logs;

        void DoLog
        (
            const String& message,
            Severity severity,
            std::optional<NameValuePairs> nameValuePairs = {}
        );
        void OnExit();
        [[nodiscard]] static File::Path OutputFilePath();
    private:
        struct Entry
        {
            String string;
            Severity severity;
            NameValuePairs nameValuePairs;
        };

        std::vector<Entry> queuedEntryList;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "LoggingCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Logging::Curator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Logging::Curator, BinaryArchive>
    {};
}