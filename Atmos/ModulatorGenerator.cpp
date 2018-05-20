
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

        GeneratorBase::GeneratorBase(const Name &name) : name(name)
        {}

        GeneratorBase::GeneratorBase(GeneratorBase &&arg) : name(std::move(arg.name))
        {}

        GeneratorBase& GeneratorBase::operator=(GeneratorBase &&arg)
        {
            name = std::move(arg.name);
            return *this;
        }

        bool GeneratorBase::operator==(const GeneratorBase &arg) const
        {
            return name == arg.name;
        }

        Observer GeneratorBase::Generate() const
        {
            return GenerateImpl();
        }

        GeneratorBase::GenerateTrackPtrT GeneratorBase::GenerateTrack(const Name &name) const
        {
            return GenerateTrackPtrT(GenerateTrackImpl(name));
        }

        const Name& GeneratorBase::GetName() const
        {
            return name;
        }
    }
}