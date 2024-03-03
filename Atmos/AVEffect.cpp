
#include "AVEffect.h"

#include "WorldManager.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AVEffect::Node::ModulatorEntry)
    {
        scribe(id);
        scribe(type);
    }

    AVEffect::Node::ModulatorEntry::ModulatorEntry(ID id, Type type) : id(id), type(type)
    {}

    AVEffect::Node::ModulatorEntry::ModulatorEntry(ModulatorEntry &&arg) : id(std::move(arg.id)), type(std::move(arg.type))
    {}

    AVEffect::Node::ModulatorEntry& AVEffect::Node::ModulatorEntry::operator=(ModulatorEntry &&arg)
    {
        id = std::move(arg.id);
        type = std::move(arg.type);
        return *this;
    }

    bool AVEffect::Node::ModulatorEntry::operator==(const ModulatorEntry &arg) const
    {
        return id == arg.id && type == arg.type;
    }

    bool AVEffect::Node::ModulatorEntry::operator!=(const ModulatorEntry &arg) const
    {
        return !(*this == arg);
    }

    void AVEffect::Node::ModulatorEntry::Stop()
    {
        //modulatorObserver->Stop();
        //modulatorObserver.Reset();
    }

    bool AVEffect::Node::ModulatorEntry::IsWorking() const
    {
        //return modulatorObserver->IsWorking();
        return true;
    }

    TimeValue AVEffect::Node::ModulatorEntry::GetSumTimeTaken() const
    {
        //return modulatorObserver->GetSumTimeTaken();
        return TimeValue();
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AVEffect::Node)
    {
        scribe(sprites);
        scribe(sounds);
        scribe(particleFountains);
        scribe(modulators);
    }

    bool AVEffect::Node::IsWorking() const
    {
        for (auto &loop : modulators)
        {
            if (loop.IsWorking())
                return true;
        }

        return false;
    }

    AVEffect::Node::Node(const Node &arg) : sprites(arg.sprites), sounds(arg.sounds), particleFountains(arg.particleFountains), modulators(arg.modulators)
    {}

    AVEffect::Node::Node(Node &&arg) : sprites(std::move(arg.sprites)), sounds(std::move(arg.sounds)), particleFountains(std::move(arg.particleFountains)), modulators(std::move(arg.modulators))
    {}

    AVEffect::Node& AVEffect::Node::operator=(const Node &arg)
    {
        sprites = arg.sprites;
        sounds = arg.sounds;
        particleFountains = arg.particleFountains;
        modulators = arg.modulators;
        return *this;
    }

    AVEffect::Node& AVEffect::Node::operator=(Node &&arg)
    {
        sprites = std::move(arg.sprites);
        sounds = std::move(arg.sounds);
        particleFountains = std::move(arg.particleFountains);
        modulators = std::move(arg.modulators);
        return *this;
    }

    bool AVEffect::Node::operator==(const Node &arg) const
    {
        return sprites == arg.sprites && sounds == arg.sounds && particleFountains == arg.particleFountains && modulators == arg.modulators;
    }

    bool AVEffect::Node::operator!=(const Node &arg) const
    {
        return !(*this == arg);
    }

    void AVEffect::Node::Start()
    {
        for (auto &loop : modulators)
            ;// loop.Start(*this);
    }

    void AVEffect::Node::Stop()
    {
        for (auto &loop : modulators)
            loop.Stop();
    }

    AVEffect::Node::SpriteHandle* AVEffect::Node::AddSprite(ID id, const SpriteHandle &add)
    {
        return &sprites.emplace(id, add).first->second;
    }

    AVEffect::Node::SpriteHandle* AVEffect::Node::AddSprite(ID id, SpriteHandle &&add)
    {
        return &sprites.emplace(id, std::move(add)).first->second;
    }

    void AVEffect::Node::RemoveSprite(ID id)
    {
        sprites.erase(id);
    }

    AVEffect::Node::SpriteHandle* AVEffect::Node::FindSprite(ID id)
    {
        auto found = sprites.find(id);
        if (found == sprites.end())
            return nullptr;

        return &found->second;
    }

    AVEffect::Node::SoundHandle* AVEffect::Node::AddSound(ID id, const SoundHandle &add)
    {
        return &sounds.emplace(id, add).first->second;
    }

    AVEffect::Node::SoundHandle* AVEffect::Node::AddSound(ID id, SoundHandle &&add)
    {
        return &sounds.emplace(id, std::move(add)).first->second;
    }

    void AVEffect::Node::RemoveSound(ID id)
    {
        sounds.erase(id);
    }

    AVEffect::Node::SoundHandle* AVEffect::Node::FindSound(ID id)
    {
        auto found = sounds.find(id);
        if (found == sounds.end())
            return nullptr;

        return &found->second;
    }

    AVEffect::Node::ParticleHandle* AVEffect::Node::AddParticleFountain(ID id, const ParticleHandle &add)
    {
        return &particleFountains.emplace(id, add).first->second;
    }

    AVEffect::Node::ParticleHandle* AVEffect::Node::AddParticleFountain(ID id, ParticleHandle &&add)
    {
        return &particleFountains.emplace(id, std::move(add)).first->second;
    }

    void AVEffect::Node::RemoveParticleFountain(ID id)
    {
        particleFountains.erase(id);
    }

    AVEffect::Node::ParticleHandle* AVEffect::Node::FindParticleFountain(ID id)
    {
        auto found = particleFountains.find(id);
        if (found == particleFountains.end())
            return nullptr;

        return &found->second;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AVEffect)
    {
        scribe(position);
        scribe(nodes);
    }

    bool AVEffect::Work()
    {
        if (!curPos->IsWorking())
        {
            ++curPos;
            if (curPos == nodes.end())
                return true;
        }

        return false;
    }

    AVEffect::AVEffect() : working(false), curPos(nodes.end())
    {}

    AVEffect::AVEffect(AVEffect &&arg) : position(std::move(arg.position)), nodes(std::move(arg.nodes)), working(false)
    {}

    AVEffect& AVEffect::operator=(AVEffect &&arg)
    {
        position = std::move(arg.position);
        nodes = std::move(arg.nodes);
        return *this;
    }

    bool AVEffect::operator==(const AVEffect &arg) const
    {
        return position == arg.position && nodes == arg.nodes;
    }

    bool AVEffect::operator!=(const AVEffect &arg) const
    {
        return !(*this == arg);
    }

    void AVEffect::Start()
    {
        if (working)
            return;

        if (nodes.empty())
            return;

        curPos = nodes.begin();
        curPos->Start();

        working = true;
        AVEffectController::Instance().effects.push_back(this);
    }

    void AVEffect::Stop()
    {
        if (!working)
            return;
        
        curPos = nodes.end();
        working = false;

        // Remove from AVEffectController
        for (auto loop = AVEffectController::Instance().effects.begin(); loop != AVEffectController::Instance().effects.end(); ++loop)
        {
            if (*loop == this)
            {
                AVEffectController::Instance().effects.erase(loop);
                break;
            }
        }
    }

    size_t AVEffect::AddNode()
    {
        return AddNode(Node());
    }

    size_t AVEffect::AddNode(Node &&node)
    {
        nodes.push_back(std::move(node));
        return nodes.size() - 1;
    }

    AVEffect::Node* AVEffect::FindNode(size_t position)
    {
        if (position >= nodes.size())
            return nullptr;

        return &*(nodes.begin() + position);
    }

    void AVEffect::RemoveNode(size_t position)
    {
        if (position >= nodes.size())
            return;

        nodes.erase(nodes.begin() + position);
    }

    void AVEffect::SetPosition(const Position3D &set)
    {
        position = set;
    }

    void AVEffect::SetX(Position3D::ValueT set)
    {
        position.SetX(set);
    }

    void AVEffect::SetY(Position3D::ValueT set)
    {
        position.SetY(set);
    }

    void AVEffect::SetZ(Position3D::ValueT set)
    {
        position.SetZ(set);
    }

    const Position3D& AVEffect::GetPosition() const
    {
        return position;
    }

    Position3D::ValueT AVEffect::GetX() const
    {
        return position.GetX();
    }

    Position3D::ValueT AVEffect::GetY() const
    {
        return position.GetY();
    }

    Position3D::ValueT AVEffect::GetZ() const
    {
        return position.GetZ();
    }

    AVEffectController::AVEffectController()
    {
        WorldManager::Instance().eventFieldSet.Subscribe(&AVEffectController::OnFieldSet);
    }

    void AVEffectController::OnFieldSet(Field &field)
    {
        Clear();
    }

    AVEffectController& AVEffectController::Instance()
    {
        static AVEffectController instance;
        return instance;
    }

    void AVEffectController::Start(AVEffect &add)
    {
        Instance().effects.push_back(&add);
    }

    void AVEffectController::Stop(AVEffect &remove)
    {
        auto &effects = Instance().effects;
        for (auto loop = effects.begin(); loop != effects.end(); ++loop)
        {
            if (*loop == &remove)
            {
                effects.erase(loop);
                return;
            }
        }
    }

    void AVEffectController::Clear()
    {
        Instance().effects.clear();
    }

    void AVEffectController::Work()
    {
        auto &effects = Instance().effects;
        for (auto loop = effects.begin(); loop != effects.end();)
        {
            if ((*loop)->Work())
                loop = effects.erase(loop);
            else
                ++loop;
        }
    }
}