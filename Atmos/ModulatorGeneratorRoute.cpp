
#include "ModulatorGeneratorRoute.h"

#include "Modulator.h"
#include "GameEnvironment.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    namespace Modulator
    {
        GeneratorRoute::TrackProxy::TrackProxy(TrackEntry *wrapped) : wrapped(wrapped)
        {}

        GeneratorRoute::TrackProxy::TrackProxy() : wrapped(nullptr)
        {}

        bool GeneratorRoute::TrackProxy::operator==(const TrackProxy &arg) const
        {
            return wrapped == arg.wrapped;
        }

        bool GeneratorRoute::TrackProxy::operator!=(const TrackProxy &arg) const
        {
            return !(*this == arg);
        }

        GeneratorRoute::TrackProxy::ID GeneratorRoute::TrackProxy::AddNode(const TrackNodeEndState &endState, const TrackNodeInterpolation &interpolation)
        {
            wrapped->nodeEntries.push_back(NodeEntry(endState, interpolation));
            return wrapped->nodeEntries.size() - 1;
        }

        void GeneratorRoute::TrackProxy::RemoveNode(ID id)
        {
            wrapped->nodeEntries.erase(wrapped->nodeEntries.begin() + id);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GeneratorRoute::NodeEntry)
        {
            scribe(endState);
            scribe(interpolation);
        }

        GeneratorRoute::NodeEntry::NodeEntry() : endState(Value::Type::INT)
        {}

        GeneratorRoute::NodeEntry::NodeEntry(const TrackNodeEndState &endState, const TrackNodeInterpolation &interpolation) : endState(endState), interpolation(interpolation)
        {}

        GeneratorRoute::NodeEntry::NodeEntry(NodeEntry &&arg) : endState(std::move(arg.endState)), interpolation(std::move(arg.interpolation))
        {}

        GeneratorRoute::NodeEntry& GeneratorRoute::NodeEntry::operator=(NodeEntry &&arg)
        {
            endState = std::move(arg.endState);
            interpolation = std::move(arg.interpolation);
            return *this;
        }

        bool GeneratorRoute::NodeEntry::operator==(const NodeEntry &arg) const
        {
            return endState == arg.endState && interpolation == arg.interpolation;
        }

        bool GeneratorRoute::NodeEntry::operator!=(const NodeEntry &arg) const
        {
            return !(*this == arg);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GeneratorRoute::TrackEntry)
        {
            scribe(name);
            scribe(nodeEntries);
        }

        GeneratorRoute::TrackEntry::TrackEntry(const Name &name) : name(name)
        {}

        GeneratorRoute::TrackEntry::TrackEntry(TrackEntry &&arg) : name(std::move(arg.name)), nodeEntries(std::move(arg.nodeEntries))
        {}

        GeneratorRoute::TrackEntry& GeneratorRoute::TrackEntry::operator=(TrackEntry &&arg)
        {
            name = std::move(arg.name);
            nodeEntries = std::move(arg.nodeEntries);
            return *this;
        }

        bool GeneratorRoute::TrackEntry::operator==(const TrackEntry &arg) const
        {
            return name == arg.name && nodeEntries == arg.nodeEntries;
        }

        bool GeneratorRoute::TrackEntry::operator!=(const TrackEntry &arg) const
        {
            return !(*this == arg);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GeneratorRoute)
        {
            scribe(modName);
            scribe(trackEntries);
        }

        GeneratorRoute::GeneratorRoute(const Name &modName) : modName(modName)
        {}

        GeneratorRoute::GeneratorRoute(GeneratorRoute &&arg) : modName(std::move(arg.modName)), trackEntries(std::move(arg.trackEntries))
        {}

        GeneratorRoute& GeneratorRoute::operator=(GeneratorRoute &&arg)
        {
            modName = std::move(arg.modName);
            trackEntries = std::move(arg.trackEntries);
            return *this;
        }

        bool GeneratorRoute::operator==(const GeneratorRoute &arg) const
        {
            return modName == arg.modName && trackEntries == arg.trackEntries;
        }

        bool GeneratorRoute::operator!=(const GeneratorRoute &arg) const
        {
            return !(*this == arg);
        }

        Observer GeneratorRoute::Generate()
        {
            auto generated = GameEnvironment::GenerateModulator(modName);
            for (auto &loop : trackEntries)
            {
                auto track = generated->AddTrack(GameEnvironment::GenerateModulatorTrack(modName, loop.name));
                for (auto &nodeLoop : loop.nodeEntries)
                {
                    auto node = track->AddNode();
                    node->SetEndState(nodeLoop.endState);
                    node->SetInterpolation(nodeLoop.interpolation);
                }
            }

            return generated;
        }

        void GeneratorRoute::SetModName(const Name &set)
        {
            modName = set;
        }

        const Name& GeneratorRoute::GetModName() const
        {
            return modName;
        }

        GeneratorRoute::TrackProxy GeneratorRoute::AddTrackEntry(const Name &name)
        {
            trackEntries.push_back(TrackEntry(name));
            return TrackProxy(&trackEntries.back());
        }

        void GeneratorRoute::RemoveTrackEntry(const TrackProxy &remove)
        {
            for (auto loop = trackEntries.begin(); loop != trackEntries.end(); ++loop)
            {
                if (&*loop == remove.wrapped)
                {
                    trackEntries.erase(loop);
                    return;
                }
            }
        }
    }
}