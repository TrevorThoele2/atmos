
#include "Modulator.h"

namespace Atmos
{
    namespace Modulator
    {
        void ModulatorBase::StartBase()
        {
            working = true;
        }

        ModulatorBase::ModulatorBase(const Name &generatorName) : generatorName(&generatorName), working(false)
        {}

        ModulatorBase::ModulatorBase(ModulatorBase &&arg) : generatorName(std::move(arg.generatorName)), working(std::move(arg.working))
        {}

        ModulatorBase& ModulatorBase::operator=(ModulatorBase &&arg)
        {
            generatorName = std::move(arg.generatorName);
            working = std::move(arg.working);
            return *this;
        }

        ModulatorBase::~ModulatorBase()
        {
            Stop();
        }

        ModulatorBase* ModulatorBase::Clone() const
        {
            return CloneImpl();
        }

        ModulatorBase::TrackID ModulatorBase::AddTrack(TrackPtr &&add)
        {
            return AddTrackImpl(std::move(add));
        }

        void ModulatorBase::RemoveTrack(TrackID id)
        {
            RemoveTrackImpl(id);
        }

        TrackBase* ModulatorBase::FindTrack(TrackID id)
        {
            return FindTrackImpl(id);
        }

        const TrackBase* ModulatorBase::FindTrack(TrackID id) const
        {
            return FindTrackImpl(id);
        }

        void ModulatorBase::Stop()
        {
            working = false;
            StopImpl();
        }

        bool ModulatorBase::Work()
        {
            bool result = WorkImpl();
            if (result)
            {
                Stop();
                return true;
            }

            return false;
        }

        bool ModulatorBase::IsWorking() const
        {
            return working;
        }

        TimeValue ModulatorBase::GetSumTimeTaken() const
        {
            return TimeValue();
        }

        const Name& ModulatorBase::GetGeneratorName() const
        {
            return *generatorName;
        }
    }
}