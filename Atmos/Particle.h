#pragma once

#include <list>

#include "Sprite.h"
#include "Modulator.h"
#include "ModulatorGeneratorRoute.h"
#include "ObjectOffsetHandle.h"

#include "Material.h"

#include "Serialization.h"

namespace Atmos
{
    class ParticleFountain
    {
    public:
        typedef TimeValue PacketSpawnRate;
        typedef unsigned short PacketSize;
    private:
        typedef Modulator::Modulator<ObjectOffsetHandle<Sprite, Position3D>> ModulatorT;

        class Piece
        {
        public:
            typedef ObjectOffsetHandle<Sprite, Position3D> HandleT;

            HandleT handle;
            FrameTimer timer;
            Modulator::Observer modulatorObserver;
            Piece(const AssetReference<Material> &material, TimeValue goal, const Position3D &position, const Modulator::Observer &modulatorObserver);

            bool operator==(const Piece &arg) const;
            bool operator!=(const Piece &arg) const;
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend class ParticleController;
    private:
        std::list<Piece> pieces;
        bool working;
        FrameTimer lifeTimer;
        // Stores the time left over from not spawning packets
        TimeValue packetTimeError;

        PacketSpawnRate nextPacketSpawnRate;

        // Returns true if it's done
        bool Work();
    public:
        Position3D position;

        // Clone objects won't ever be used directly, but will be used for each new sprite
        AssetReference<Material> cloneMaterial;
        Sprite::Index cloneIndex;
        Modulator::GeneratorRoute cloneModulator;

        // How long the particles will last
        typedef Range<TimeValue> ParticleLifetimeRange;
        ParticleLifetimeRange particleLifetime;

        // How many particles will spawn per packet
        typedef Range<PacketSize> PacketSizeRange;
        PacketSizeRange packetSize;

        // How often, in seconds, that the packets will spawn
        typedef Range<PacketSpawnRate> PacketSpawnRateRange;
        PacketSpawnRateRange packetSpawnRate;

        // Lifetime of the particle fountain itself
        typedef Range<TimeValue> LifetimeRange;
        LifetimeRange lifetime;

        ParticleFountain();
        bool operator==(const ParticleFountain &arg) const;
        bool operator!=(const ParticleFountain &arg) const;

        void Start();
        void Stop();
        bool IsWorking() const;

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
    class ParticleController
    {
    private:
        friend ParticleFountain;
    private:
        std::vector<ParticleFountain*> fountains;

        ParticleController();
        ParticleController(const ParticleController &arg) = delete;
        ParticleController& operator=(const ParticleController &arg) = delete;

        static void OnFieldSet(Field &field);

        static void Add(ParticleFountain &add);
        static void Remove(ParticleFountain &remove);
    public:
        static ParticleController& Instance();

        static void Clear();
        static void Work();
    };
}