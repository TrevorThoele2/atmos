#pragma once

#include <Arca/Curator.h>
#include "ProcessedLog.h"

#include <Arca/TransferableSignal.h>
#include <Arca/SignalBatch.h>

namespace Atmos::Logging
{
    class Transfer : public Arca::Curator
    {
    public:
        explicit Transfer(Init init, Arca::Reliquary& destination);
        explicit Transfer(Init init, const std::vector<Arca::Reliquary*>& destinations);

        void Work();
    private:
        Arca::Batch<Arca::TransferableSignal> transferableSignals;
    private:
        std::vector<Arca::Reliquary*> destinations;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::Transfer>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Logging::Transfer";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Logging::Transfer, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Logging::Transfer, BinaryArchive>
    {};
}