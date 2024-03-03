
#include "ModulatorTrack.h"

namespace Atmos
{
    namespace Modulator
    {
        TrackBase::~TrackBase()
        {}

        TrackBase* TrackBase::Clone() const
        {
            return CloneImpl();
        }

        TrackNode* TrackBase::AddNode()
        {
            return AddNodeImpl();
        }

        void TrackBase::RemoveNode(ID id)
        {
            RemoveNodeImpl(id);
        }

        TrackNode* TrackBase::FindNode(ID id)
        {
            return FindNodeImpl(id);
        }

        const TrackNode* TrackBase::FindNode(ID id) const
        {
            return FindNodeImpl(id);
        }

        bool TrackBase::HasNode(ID id) const
        {
            return FindNodeImpl(id) != nullptr;
        }

        TimeValue TrackBase::GetSumTimeTaken() const
        {
            return GetSumTimeTakenImpl();
        }
    }
}