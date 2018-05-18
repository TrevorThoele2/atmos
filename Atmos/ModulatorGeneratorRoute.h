#pragma once

#include <vector>
#include "Name.h"
#include "ModulatorTrackNode.h"
#include "ModulatorObserver.h"

namespace Atmos
{
    namespace Modulator
    {
        class ModulatorBase;

        // Defines a "route" to a modulator generator, IE binds strings and end states together to generate a modulator
        // Use these to store the ability to generate modulators later with the help of the actual generators
        class GeneratorRoute
        {
        private:
            class TrackEntry;
        public:
            class TrackProxy
            {
            public:
                typedef size_t ID;
            private:
                friend GeneratorRoute;
            private:
                TrackEntry *wrapped;

                TrackProxy(TrackEntry *wrapped);
            public:
                TrackProxy();
                TrackProxy(const TrackProxy&arg) = default;
                TrackProxy& operator=(const TrackProxy &arg) = default;
                bool operator==(const TrackProxy &arg) const;
                bool operator!=(const TrackProxy &arg) const;

                ID AddNode(const TrackNodeEndState &endState, const TrackNodeInterpolation &interpolation);
                void RemoveNode(ID id);
            };
        private:
            class NodeEntry
            {
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            public:
                TrackNodeEndState endState;
                TrackNodeInterpolation interpolation;
                NodeEntry();
                NodeEntry(const TrackNodeEndState &endState, const TrackNodeInterpolation &interpolation);
                NodeEntry(const NodeEntry &arg) = default;
                NodeEntry(NodeEntry &&arg);
                NodeEntry& operator=(const NodeEntry &arg) = default;
                NodeEntry& operator=(NodeEntry &&arg);
                bool operator==(const NodeEntry &arg) const;
                bool operator!=(const NodeEntry &arg) const;
            };

            class TrackEntry
            {
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            public:
                Name name;
                std::vector<NodeEntry> nodeEntries;
                TrackEntry() = default;
                TrackEntry(const Name &name);
                TrackEntry(const TrackEntry &arg) = default;
                TrackEntry(TrackEntry &&arg);
                TrackEntry& operator=(const TrackEntry &arg) = default;
                TrackEntry& operator=(TrackEntry &&arg);
                bool operator==(const TrackEntry &arg) const;
                bool operator!=(const TrackEntry &arg) const;
            };
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            Name modName;
            std::vector<TrackEntry> trackEntries;
        public:
            GeneratorRoute() = default;
            GeneratorRoute(const Name &modName);
            GeneratorRoute(const GeneratorRoute &arg) = default;
            GeneratorRoute(GeneratorRoute &&arg);
            GeneratorRoute& operator=(const GeneratorRoute &arg) = default;
            GeneratorRoute& operator=(GeneratorRoute &&arg);
            bool operator==(const GeneratorRoute &arg) const;
            bool operator!=(const GeneratorRoute &arg) const;

            Observer Generate();

            void SetModName(const Name &set);
            const Name& GetModName() const;
            TrackProxy AddTrackEntry(const Name &name);
            void RemoveTrackEntry(const TrackProxy &remove);
        };
    }
}