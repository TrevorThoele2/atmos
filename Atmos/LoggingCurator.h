#pragma once

#include <Arca/Curator.h>

#include "Log.h"
#include "LoggingSeverity.h"
#include "LoggingDetails.h"

#include "FilePath.h"

namespace Atmos::Logging
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);
    public:
        void Handle(const Log& log);
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
        using HandledCommands = Arca::HandledCommands<Atmos::Logging::Log>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Logging::Curator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Logging::Curator, BinaryArchive>
    {};
}