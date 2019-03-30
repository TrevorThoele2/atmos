#pragma once

#include "Object.h"

#include "nModulatorTrack.h"
#include "nModulatorLink.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Modulator
    {
        class Modulator : public Object
        {
        public:
            typedef TypedObjectReference<Track> TrackReference;
        public:
            Modulator();
            Modulator(const Modulator& arg) = default;
            Modulator(const ::Inscription::Table<Modulator>& table);

            TrackReference AddTrack(const Link& link);

            void Start();
            void Work();

            ObjectTypeDescription TypeDescription() const override;
        private:
            typedef std::vector<TrackReference> TrackList;
            TrackList tracks;
            TrackList::iterator curTrack;
        private:
            bool CanWork() const;
        };
    }

    template<>
    struct ObjectTraits<Modulator::Modulator> : ObjectTraitsBase<Modulator::Modulator>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Modulator::Modulator)
    {
    public:
        static void AddMembers(TableT& table);
    };
}