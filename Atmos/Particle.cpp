
#include "Particle.h"
#include "WorldManager.h"
#include "Environment.h"
#include <Inscription/Scribe.h>

namespace Atmos
{
    ParticleFountain::Piece::Piece(const AssetReference<Material> &material, TimeValue goal, const Position3D &position) : timer(goal), handle(position)
    {
        handle->SetMaterial(material);
    }

    bool ParticleFountain::Piece::operator==(const Piece &arg) const
    {
        return handle == arg.handle && timer == arg.timer;
    }

    bool ParticleFountain::Piece::operator!=(const Piece &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ParticleFountain)
    {
        scribe(cloneMaterial);
        scribe(cloneIndex);

        scribe(particleLifetime);
        scribe(packetSize);
        scribe(packetSpawnRate);
        scribe(lifetime);
    }

    bool ParticleFountain::Work()
    {
        if (!IsWorking() || lifeTimer.HasReachedGoal())
            return true;

        // Check if should erase the piece and update the sprite's position
        for (auto pieceLoop = pieces.begin(); pieceLoop != pieces.end();)
        {
            if (!pieceLoop->timer.HasReachedGoal())
            {
                ++pieceLoop;
                continue;
            }
            
            pieceLoop = pieces.erase(pieceLoop);
        }

        // Add the last frame's time to the error
        packetTimeError += Environment::GetTime().GetLastElapsed();
        auto convertedNextPacketSpawnRate = nextPacketSpawnRate.ConvertValue(TimeValueEpoch::NANOSECONDS);
        if (packetTimeError >= convertedNextPacketSpawnRate)
        {
            while (packetTimeError >= convertedNextPacketSpawnRate)
            {
                // Spawn a packet
                PacketSize size = Random::Generate(packetSize);
                while (size-- > 0)
                {
                    //pieces.push_back(Piece(cloneMaterial, Random::Generate(particleLifetime.GetBegin().Get(), particleLifetime.GetEnd().Get()), position, cloneModulator.Generate()));
                    //static_cast<ModulatorT*>(pieces.back().modulatorObserver.Get())->Start(pieces.back().handle);
                }

                // Lower the error
                packetTimeError -= convertedNextPacketSpawnRate;
            }

            // Create the next packet spawn rate
            nextPacketSpawnRate = Random::Generate(packetSpawnRate.GetBegin().Get(), packetSpawnRate.GetEnd().Get());
        }

        // Skip the manipulators if the pieces are empty
        if (pieces.empty())
            return false;

        return false;
    }

    ParticleFountain::ParticleFountain() : working(false), packetTimeError(), cloneIndex(1), particleLifetime(), packetSize(0, 0), packetSpawnRate(), lifetime()
    {}

    bool ParticleFountain::operator==(const ParticleFountain &arg) const
    {
        return pieces == arg.pieces && working == arg.working && lifeTimer == arg.lifeTimer && packetTimeError == arg.packetTimeError && nextPacketSpawnRate == arg.nextPacketSpawnRate && position == arg.position && 
            cloneMaterial == arg.cloneMaterial && cloneIndex == arg.cloneIndex && particleLifetime == arg.particleLifetime &&
            packetSize == arg.packetSize && packetSpawnRate == arg.packetSpawnRate && lifetime == arg.lifetime;
    }

    bool ParticleFountain::operator!=(const ParticleFountain &arg) const
    {
        return !(*this == arg);
    }

    void ParticleFountain::Start()
    {
        if (IsWorking())
            return;

        // Determine the lifetime of this
        lifeTimer.SetGoal(Random::Generate(lifetime.GetBegin().Get(), lifetime.GetEnd().Get()));
        lifeTimer.Start();

        working = true;
    }

    void ParticleFountain::Stop()
    {
        if (!IsWorking())
            return;

        for (auto &loop : pieces)
            ;// loop.modulatorObserver->Stop();

        working = false;
    }

    bool ParticleFountain::IsWorking() const
    {
        return working;
    }

    void ParticleFountain::SetPosition(const Position3D &set)
    {
        position = set;
    }

    void ParticleFountain::SetX(Position3D::ValueT set)
    {
        position.SetX(set);
    }

    void ParticleFountain::SetY(Position3D::ValueT set)
    {
        position.SetY(set);
    }

    void ParticleFountain::SetZ(Position3D::ValueT set)
    {
        position.SetZ(set);
    }

    const Position3D& ParticleFountain::GetPosition() const
    {
        return position;
    }

    Position3D::ValueT ParticleFountain::GetX() const
    {
        return position.GetX();
    }

    Position3D::ValueT ParticleFountain::GetY() const
    {
        return position.GetY();
    }

    Position3D::ValueT ParticleFountain::GetZ() const
    {
        return position.GetZ();
    }

    ParticleController::ParticleController()
    {
        WorldManager::Instance().eventFieldSet.Subscribe(&ParticleController::OnFieldSet);
    }

    void ParticleController::OnFieldSet(Field &field)
    {
        Clear();
    }

    void ParticleController::Add(ParticleFountain &add)
    {
        Instance().fountains.push_back(&add);
    }

    void ParticleController::Remove(ParticleFountain &remove)
    {
        auto &fountains = Instance().fountains;
        for (auto loop = fountains.begin(); loop != fountains.end(); ++loop)
        {
            if (*loop == &remove)
            {
                fountains.erase(loop);
                return;
            }
        }
    }

    ParticleController& ParticleController::Instance()
    {
        static ParticleController instance;
        return instance;
    }

    void ParticleController::Clear()
    {
        Instance().fountains.clear();
    }

    void ParticleController::Work()
    {
        auto &fountains = Instance().fountains;
        auto loop = fountains.begin();
        while (loop != fountains.end())
        {
            if ((*loop)->Work())
                loop = fountains.erase(loop);
            else
                ++loop;
        }
    }
}