#pragma once

#include <vector>

#include "Object.h"
#include "ObjectReference.h"

#include "nModulatorNode.h"
#include "nModulatorLink.h"
#include "ModulatorDelta.h"

#include "FrameTimer.h"

#include "Optional.h"

namespace Atmos
{
    namespace Modulator
    {
        class Track : public Object
        {
        public:
            typedef TypedObjectReference<Node> NodeReference;
        private:
            typedef std::vector<NodeReference> NodeList;
        public:
            typedef NodeList::iterator iterator;
            typedef NodeList::const_iterator const_iterator;
        public:
            Value startValue;
            TimeValue timeTaken;
        public:
            Track(const Link& link);
            Track(const Track& arg) = default;
            Track(const ::Inscription::Table<Track>& table);

            iterator AddNode();
            void RemoveNode(iterator itr);
            void RemoveNode(const_iterator itr);

            void Start();
            void Work();

            bool IsDone() const;

            iterator begin();
            const_iterator begin() const;
            iterator end();
            const_iterator end() const;

            ObjectTypeDescription TypeDescription() const override;
        private:
            Link link;
            Value curStartingValue;

            Value CurrentValue(Delta delta) const;
            Delta CurrentDelta() const;
        private:
            NodeList nodes;
            iterator curNode;
        private:
            FrameTimer timer;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Modulator::Track> : ObjectTraitsBase<Modulator::Track>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Modulator::Track)
    {
    public:
        static void AddMembers(TableT& table);
    };
}