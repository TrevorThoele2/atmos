#include "BoundsCurator.h"

#include "Bounds.h"

namespace Atmos
{
    BoundsCurator::BoundsCurator(Init init) : Curator(init)
    {}

    void BoundsCurator::Handle(const MoveBounds& command)
    {
        auto bounds = MutablePointer().Of<Bounds>(command.id);
        bounds->Position(command.toPosition);
    }

    void BoundsCurator::Handle(const ScaleBounds& command)
    {
        auto bounds = MutablePointer().Of<Bounds>(command.id);
        bounds->Scalers(command.toScalers);
    }
}