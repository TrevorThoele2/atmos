
#include "ModulatorGenerator.h"

namespace Atmos
{
    namespace Modulator
    {
        TrackGeneratorBase* TrackGeneratorBase::Clone() const
        {
            return CloneImpl();
        }

        TrackGeneratorBase::GeneratePtrT TrackGeneratorBase::Generate() const
        {
            return GenerateImpl();
        }

        Observer GeneratorBase::Generate() const
        {
            return GenerateImpl();
        }

        GeneratorBase::GenerateTrackPtrT GeneratorBase::GenerateTrack(const Name &name) const
        {
            return GenerateTrackPtrT(GenerateTrackImpl(name));
        }
    }
}