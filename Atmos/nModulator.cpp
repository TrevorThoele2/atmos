
#include "nModulator.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Modulator
    {
        Modulator::Modulator() : curTrack(tracks.end())
        {}

        Modulator::Modulator(const ::Inscription::Table<Modulator>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
        {}

        Modulator::TrackReference Modulator::AddTrack(const Link& link)
        {
            auto track = Manager()->CreateObject<Track>(link);
            tracks.push_back(track);
            return track;
        }

        void Modulator::Start()
        {
            if (tracks.empty())
            {
                curTrack = tracks.end();
                return;
            }

            curTrack = tracks.begin();
            (*curTrack)->Start();
        }

        void Modulator::Work()
        {
            if (!CanWork())
                return;

            (*curTrack)->Work();
            if ((*curTrack)->IsDone())
                ++curTrack;
        }

        ObjectTypeDescription Modulator::TypeDescription() const
        {
            return ObjectTraits<Modulator>::TypeDescription();
        }

        bool Modulator::CanWork() const
        {
            return curTrack != tracks.end();
        }
    }

    const ObjectTypeName ObjectTraits<Modulator::Modulator>::typeName = "Modulator";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Modulator::Modulator)
    {

    }
}