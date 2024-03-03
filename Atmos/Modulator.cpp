
#include "Modulator.h"

#include "GameEnvironment.h"

namespace Atmos
{
    namespace Modulator
    {
        void ModulatorBase::StartBase()
        {
            working = true;
        }

        ModulatorBase::ModulatorBase() : working(false)
        {}

        ModulatorBase::ModulatorBase(ModulatorBase &&arg) : working(std::move(arg.working))
        {}

        ModulatorBase& ModulatorBase::operator=(ModulatorBase &&arg)
        {
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

        TrackBase* ModulatorBase::AddTrack(TrackPtr &&add)
        {
            return AddTrackImpl(std::move(add));
        }

        void ModulatorBase::RemoveTrack(ID id)
        {
            RemoveTrackImpl(id);
        }

        TrackBase* ModulatorBase::FindTrack(ID id)
        {
            return FindTrackImpl(id);
        }

        const TrackBase* ModulatorBase::FindTrack(ID id) const
        {
            return FindTrackImpl(id);
        }

        void ModulatorBase::Stop()
        {
            GameEnvironment::DetachModulator(*this);
        }

        bool ModulatorBase::Work()
        {
            bool result = WorkImpl();
            if (result)
            {
                working = false;
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
    }
}