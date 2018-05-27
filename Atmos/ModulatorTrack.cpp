
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

        TrackBase::NodeID TrackBase::AddNode()
        {
            return AddNodeImpl();
        }

        void TrackBase::RemoveNode(NodeID id)
        {
            RemoveNodeImpl(id);
        }

        TrackNode* TrackBase::FindNode(NodeID id)
        {
            return FindNodeImpl(id);
        }

        const TrackNode* TrackBase::FindNode(NodeID id) const
        {
            return FindNodeImpl(id);
        }

        bool TrackBase::HasNode(NodeID id) const
        {
            return FindNodeImpl(id) != nullptr;
        }

        TimeValue TrackBase::GetSumTimeTaken() const
        {
            return GetSumTimeTakenImpl();
        }
    }
}