#pragma once

#include <vector>

#include "Sprite.h"
#include "Sound.h"
#include "Particle.h"

#include "ModulatorGeneratorRoute.h"

#include "OffsetMovement.h"

#include "ObjectOffsetHandle.h"
#include "Position3D.h"

namespace Atmos
{
    class AVEffect
    {
    public:
        class Node
        {
        public:
            // ID used for the modulator to find the object
            typedef size_t ID;

            // Handles
            typedef ObjectOffsetHandle<Sprite, Position3D> SpriteHandle;
            typedef ObjectOffsetHandle<Sound, Position3D> SoundHandle;
            typedef ObjectOffsetHandle<ParticleFountain, Position3D> ParticleHandle;

            // Modulators
            typedef Modulator::Modulator<SpriteHandle> SpriteModulator;
            typedef Modulator::Modulator<SoundHandle> SoundModulator;
            typedef Modulator::Modulator<ParticleHandle> ParticleModulator;

            enum class ModulatorEntryType
            {
                SPRITE,
                SOUND,
                PARTICLE
            };

            class ModulatorEntry
            {
            public:
                typedef ModulatorEntryType Type;
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            private:
                ID id;
                Type type;
                Modulator::GeneratorRoute generatorRoute;
                Modulator::Observer modulatorObserver;
            public:
                ModulatorEntry() = default;
                ModulatorEntry(ID id, Type type, const Modulator::GeneratorRoute &generatorRoute);
                ModulatorEntry(ID id, Type type, Modulator::GeneratorRoute &&generatorRoute);
                ModulatorEntry(const ModulatorEntry &arg) = default;
                ModulatorEntry(ModulatorEntry &&arg);
                ModulatorEntry& operator=(const ModulatorEntry &arg) = default;
                ModulatorEntry& operator=(ModulatorEntry &&arg);
                bool operator==(const ModulatorEntry &arg) const;
                bool operator!=(const ModulatorEntry &arg) const;

                Modulator::Observer Start(Node &node);
                void Stop();
                bool IsWorking() const;
                TimeValue GetSumTimeTaken() const;
            };

            typedef std::unordered_map<Name, ModulatorEntry> ModulatorMap;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            friend AVEffect;
        private:
            bool IsWorking() const;
        public:
            // Handles
            std::unordered_map<ID, SpriteHandle> sprites;
            std::unordered_map<ID, SoundHandle> sounds;
            std::unordered_map<ID, ParticleHandle> particleFountains;

            // Modulators
            std::vector<ModulatorEntry> modulators;

            Node() = default;
            Node(const Node &arg);
            Node(Node &&arg);
            Node& operator=(const Node &arg);
            Node& operator=(Node &&arg);

            bool operator==(const Node &arg) const;
            bool operator!=(const Node &arg) const;

            void Start();
            void Stop();

            SpriteHandle* AddSprite(ID id, const SpriteHandle &add);
            SpriteHandle* AddSprite(ID id, SpriteHandle &&add);
            void RemoveSprite(ID id);
            SpriteHandle* FindSprite(ID id);

            SoundHandle* AddSound(ID id, const SoundHandle &add);
            SoundHandle* AddSound(ID id, SoundHandle &&add);
            void RemoveSound(ID id);
            SoundHandle* FindSound(ID id);

            ParticleHandle* AddParticleFountain(ID id, const ParticleHandle &add);
            ParticleHandle* AddParticleFountain(ID id, ParticleHandle &&add);
            void RemoveParticleFountain(ID id);
            ParticleHandle* FindParticleFountain(ID id);
        };

        typedef std::vector<Node> NodeContainer;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend class AVEffectController;
    private:
        bool working;
        NodeContainer::iterator curPos;

        // Returns true if this is done
        bool Work();
    public:
        Position3D position;
        NodeContainer nodes;

        AVEffect();
        AVEffect(const AVEffect &arg) = default;
        AVEffect(AVEffect &&arg);
        AVEffect& operator=(const AVEffect &arg) = default;
        AVEffect& operator=(AVEffect &&arg);
        bool operator==(const AVEffect &arg) const;
        bool operator!=(const AVEffect &arg) const;

        void Start();
        void Stop();

        size_t AddNode();
        size_t AddNode(Node &&node);
        Node* FindNode(size_t position);
        void RemoveNode(size_t position);

        void SetPosition(const Position3D &set);
        void SetX(Position3D::ValueT set);
        void SetY(Position3D::ValueT set);
        void SetZ(Position3D::ValueT set);
        const Position3D& GetPosition() const;
        Position3D::ValueT GetX() const;
        Position3D::ValueT GetY() const;
        Position3D::ValueT GetZ() const;
    };

    class Field;
    class AVEffectController
    {
    private:
        friend AVEffect;
    private:
        std::vector<AVEffect*> effects;
        AVEffectController();
        AVEffectController(const AVEffectController &arg) = delete;
        AVEffectController& operator=(const AVEffectController &arg) = delete;

        static void OnFieldSet(Field &field);
    public:
        static AVEffectController& Instance();
        static void Start(AVEffect &start);
        static void Stop(AVEffect &stop);
        static void Clear();
        static void Work();
    };
}