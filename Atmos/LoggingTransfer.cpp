#include "LoggingTransfer.h"

#include <Arca/Reliquary.h>

namespace Atmos::Logging
{
    Transfer::Transfer(Init init, Arca::Reliquary& destination) :
        Curator(init), transferableSignals(init.owner.Batch<Arca::TransferableSignal>()),
        destinations({ &destination })
    {}

    Transfer::Transfer(Init init, const std::vector<Arca::Reliquary*>& destinations) :
        Curator(init), transferableSignals(init.owner.Batch<Arca::TransferableSignal>()),
        destinations(destinations)
    {
        for(auto destination = this->destinations.begin(); destination != this->destinations.end();)
        {
            if (*destination == nullptr)
                destination = this->destinations.erase(destination);
            else
                ++destination;
        }
    }

    void Transfer::Work()
    {
        for (auto& signal : transferableSignals)
        {
            if (signal.TransferringTypeName() != Arca::Traits<ProcessedLog>::typeName)
                continue;

            for (auto& destination : destinations)
                signal.Raise(*destination);
        }
    }
}