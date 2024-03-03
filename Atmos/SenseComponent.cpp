
#include "SenseComponent.h"
#include "Entity.h"

#include "Field.h"

#include "CurrentField.h"

#include "Error.h"

#include <Inscription/Inscripter.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(SenseComponent::ModulatorEntry)
        {
            scribe(id);
            scribe(type);
            scribe(generatorRoute);
        }

        SenseComponent::ModulatorEntry::ModulatorEntry() : id(0), type(Type::SPRITE), generatorRoute()
        {}

        SenseComponent::ModulatorEntry::ModulatorEntry(ObjectID id, Type type, const Modulator::GeneratorRoute &generatorRoute) : id(id), type(type), generatorRoute(generatorRoute)
        {}

        SenseComponent::ModulatorEntry::ModulatorEntry(ObjectID id, Type type, Modulator::GeneratorRoute &&generatorRoute) : id(id), type(type), generatorRoute(std::move(generatorRoute))
        {}

        SenseComponent::ModulatorEntry::ModulatorEntry(ModulatorEntry &&arg) : id(std::move(arg.id)), type(std::move(arg.type)), generatorRoute(std::move(arg.generatorRoute))
        {}

        SenseComponent::ModulatorEntry& SenseComponent::ModulatorEntry::operator=(ModulatorEntry &&arg)
        {
            id = std::move(arg.id);
            type = std::move(arg.type);
            generatorRoute = std::move(arg.generatorRoute);
            return *this;
        }

        bool SenseComponent::ModulatorEntry::operator==(const ModulatorEntry &arg) const
        {
            return id == arg.id && type == arg.type && generatorRoute == arg.generatorRoute;
        }

        bool SenseComponent::ModulatorEntry::operator!=(const ModulatorEntry &arg) const
        {
            return !(*this == arg);
        }

        Modulator::Observer SenseComponent::ModulatorEntry::Start(SenseComponent &component)
        {
            modulatorObserver = generatorRoute.Generate();

            switch (type)
            {
            case ModulatorEntryType::SPRITE:
                static_cast<SpriteModulator*>(modulatorObserver.Get())->Start(component.sprites.Find(id)->second);
                break;
            case ModulatorEntryType::SOUND:
                static_cast<SoundModulator*>(modulatorObserver.Get())->Start(component.sounds.Find(id)->second);
                break;
            case ModulatorEntryType::AV_EFFECT:
                static_cast<AVEffectModulator*>(modulatorObserver.Get())->Start(component.avEffects.Find(id)->second);
                break;
            }

            return modulatorObserver;
        }

        void SenseComponent::ModulatorEntry::Stop()
        {
            if (!modulatorObserver.IsValid())
                return;

            modulatorObserver->Stop();
            modulatorObserver.Reset();
        }

        bool SenseComponent::ModulatorEntry::IsWorking() const
        {
            if (!modulatorObserver.IsValid())
                return false;
            else
                return modulatorObserver->IsWorking();
        }

        TimeValue SenseComponent::ModulatorEntry::GetSumTimeTaken() const
        {
            if (!modulatorObserver.IsValid())
                return TimeValue();
            else
                return modulatorObserver->GetSumTimeTaken();
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(SenseComponent::ModulatorPack)
        {
            scribe(entries);
        }

        SenseComponent::ModulatorPack::ModulatorPack(ModulatorPack &&arg) : entries(std::move(arg.entries))
        {}

        SenseComponent::ModulatorPack& SenseComponent::ModulatorPack::operator=(ModulatorPack &&arg)
        {
            entries = std::move(arg.entries);
            return *this;
        }

        bool SenseComponent::ModulatorPack::operator==(const ModulatorPack &arg) const
        {
            return entries == arg.entries;
        }

        bool SenseComponent::ModulatorPack::operator!=(const ModulatorPack &arg) const
        {
            return !(*this == arg);
        }

        void SenseComponent::ModulatorPack::StartAll(SenseComponent &component)
        {
            if (IsWorking())
                return;

            for (auto &loop : entries)
                loop.Start(component);
        }

        void SenseComponent::ModulatorPack::StopAll()
        {
            if (!IsWorking())
                return;

            for (auto &loop : entries)
                loop.Stop();
        }

        bool SenseComponent::ModulatorPack::IsWorking() const
        {
            for (auto &loop : entries)
            {
                if (loop.IsWorking())
                    return true;
            }

            return false;
        }

        TimeValue SenseComponent::ModulatorPack::GetTimeTaken() const
        {
            TimeValue ret;
            for (auto &loop : entries)
            {
                auto checkTimeTaken = loop.GetSumTimeTaken();
                if (checkTimeTaken > ret)
                    ret = checkTimeTaken;
            }

            return ret;
        }

        SenseComponent::ModulatorEntry* SenseComponent::ModulatorPack::Add(EntryID id, ModulatorEntryType type)
        {
            return Add(ModulatorEntry(id, type, Modulator::GeneratorRoute()));
        }

        SenseComponent::ModulatorEntry* SenseComponent::ModulatorPack::Add(EntryID id, ModulatorEntryType type, const Modulator::GeneratorRoute &generatorRoute)
        {
            return Add(ModulatorEntry(id, type, generatorRoute));
        }

        SenseComponent::ModulatorEntry* SenseComponent::ModulatorPack::Add(EntryID id, ModulatorEntryType type, Modulator::GeneratorRoute &&generatorRoute)
        {
            return Add(ModulatorEntry(id, type, std::move(generatorRoute)));
        }

        SenseComponent::ModulatorEntry* SenseComponent::ModulatorPack::Add(const ModulatorEntry &add)
        {
            entries.push_back(add);
            return &entries.back();
        }

        SenseComponent::ModulatorEntry* SenseComponent::ModulatorPack::Add(ModulatorEntry &&add)
        {
            entries.push_back(std::move(add));
            return &entries.back();
        }

        void SenseComponent::ModulatorPack::Remove(EntryID remove)
        {
            entries.erase(entries.begin() + remove);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(SenseComponent)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(position);
                scribe.Save(sprites);

                scribe.Save(modulatorPacks);

                scribe.Save(show);
            }
            else // INPUT
            {
                scribe.Load(position);
                scribe.Load(sprites);
                sprites.SetOwnerPosition(&position);

                scribe.Load(modulatorPacks);

                scribe.Load(show);

                SyncObjects();
            }
        }

        SenseComponent::SenseComponent() : show(true)
        {}

        SenseComponent::SenseComponent(const SenseComponent &arg) : position(arg.position), sprites(arg.sprites, &position), sounds(arg.sounds, &position), avEffects(arg.avEffects, &position), modulatorPacks(arg.modulatorPacks), show(arg.show)
        {
            SyncObjects();
        }

        SenseComponent& SenseComponent::operator=(const SenseComponent &arg)
        {
            position = arg.position;
            sprites = arg.sprites;
            sounds = arg.sounds;
            avEffects = arg.avEffects;
            modulatorPacks = arg.modulatorPacks;
            show = arg.show;
            SyncObjects();
            return *this;
        }

        SenseComponent::SenseComponent(SenseComponent &&arg) : position(std::move(arg.position)), sprites(std::move(arg.sprites), &position), sounds(std::move(arg.sounds), &position), avEffects(std::move(arg.avEffects), &position), modulatorPacks(std::move(arg.modulatorPacks)), show(std::move(arg.show))
        {
            SyncObjects();
        }

        SenseComponent& SenseComponent::operator=(SenseComponent &&arg)
        {
            position = std::move(arg.position);
            sprites = std::move(arg.sprites);
            sounds = std::move(arg.sounds);
            avEffects = std::move(arg.avEffects);
            modulatorPacks = std::move(arg.modulatorPacks);
            show = arg.show;
            SyncObjects();
            return *this;
        }

        bool SenseComponent::operator==(const SenseComponent &arg) const
        {
            return position == arg.position && sprites == arg.sprites && sounds == arg.sounds && avEffects == arg.avEffects && modulatorPacks == arg.modulatorPacks && show == arg.show;
        }

        bool SenseComponent::operator!=(const SenseComponent &arg) const
        {
            return !(*this == arg);
        }

        SenseComponent::SpriteList::ID SenseComponent::AddSprite()
        {
            auto added = AddImpl(sprites);
            if(show)
                GetCurrentRenderFragments()->Add(FindSprite(added)->Get());
            return added;
        }

        SenseComponent::SpriteList::ID SenseComponent::AddSprite(SpriteList::ID id)
        {
            auto added = AddImpl(id, sprites);
            if(show)
                GetCurrentRenderFragments()->Add(FindSprite(added)->Get());
            return added;
        }

        SenseComponent::SpriteList::ID SenseComponent::AddSprite(const Sprite &add)
        {
            auto added = AddImpl(add, sprites);
            if (show)
                GetCurrentRenderFragments()->Add(FindSprite(added)->Get());
            return added;
        }

        SenseComponent::SpriteList::ID SenseComponent::AddSprite(Sprite &&add, SpriteList::OffsetValueT offsetX, SpriteList::OffsetValueT offsetY, SpriteList::OffsetValueT offsetZ)
        {
            auto added = AddImpl(std::move(add), offsetX, offsetY, offsetZ, sprites);
            if (show)
                GetCurrentRenderFragments()->Add(FindSprite(added)->Get());
            return added;
        }

        SenseComponent::SpriteList::ID SenseComponent::AddSprite(SpriteList::ID id, Sprite &&add, SpriteList::OffsetValueT offsetX, SpriteList::OffsetValueT offsetY, SpriteList::OffsetValueT offsetZ)
        {
            auto added = AddImpl(id, std::move(add), offsetX, offsetY, offsetZ, sprites);
            if (show)
                GetCurrentRenderFragments()->Add(FindSprite(added)->Get());
            return added;
        }

        void SenseComponent::RemoveSprite(const Sprite &remove)
        {
            RemoveByObjectImpl(remove, sprites, [](Sprite &sprite) { GetCurrentRenderFragments()->Remove(sprite); });
        }

        void SenseComponent::RemoveSprite(SpriteList::ID id)
        {
            RemoveByIDImpl(id, sprites, [](Sprite &sprite) { GetCurrentRenderFragments()->Remove(sprite); });
        }

        SenseComponent::SpriteHandle* SenseComponent::FindSprite(SpriteList::ID id)
        {
            return FindImpl(id, sprites);
        }

        const SenseComponent::SpriteHandle* SenseComponent::FindSprite(SpriteList::ID id) const
        {
            return FindImpl(id, sprites);
        }

        SenseComponent::SoundList::ID SenseComponent::AddSound()
        {
            return AddImpl(sounds);
        }

        SenseComponent::SoundList::ID SenseComponent::AddSound(SoundList::ID id)
        {
            return AddImpl(id, sounds);
        }

        SenseComponent::SoundList::ID SenseComponent::AddSound(const Sound &add)
        {
            return AddImpl(add, sounds);
        }

        SenseComponent::SoundList::ID SenseComponent::AddSound(Sound &&add, SoundList::OffsetValueT offsetX, SoundList::OffsetValueT offsetY, SoundList::OffsetValueT offsetZ)
        {
            return AddImpl(std::move(add), offsetX, offsetY, offsetZ, sounds);
        }

        SenseComponent::SoundList::ID SenseComponent::AddSound(SoundList::ID id, Sound &&add, SoundList::OffsetValueT offsetX, SoundList::OffsetValueT offsetY, SoundList::OffsetValueT offsetZ)
        {
            return AddImpl(id, std::move(add), offsetX, offsetY, offsetZ, sounds);
        }

        void SenseComponent::RemoveSound(const Sound &remove)
        {
            RemoveByObjectImpl(remove, sounds);
        }

        void SenseComponent::RemoveSound(SoundList::ID id)
        {
            RemoveByIDImpl(id, sounds);
        }

        SenseComponent::SoundHandle* SenseComponent::FindSound(SoundList::ID id)
        {
            return FindImpl(id, sounds);
        }

        const SenseComponent::SoundHandle* SenseComponent::FindSound(SoundList::ID id) const
        {
            return FindImpl(id, sounds);
        }

        SenseComponent::AVEffectList::ID SenseComponent::AddAVEffect()
        {
            return AddImpl(avEffects);
        }

        SenseComponent::AVEffectList::ID SenseComponent::AddAVEffect(AVEffectList::ID id)
        {
            return AddImpl(id, avEffects);
        }

        SenseComponent::AVEffectList::ID SenseComponent::AddAVEffect(const AVEffect &add)
        {
            return AddImpl(add, avEffects);
        }

        SenseComponent::AVEffectList::ID SenseComponent::AddAVEffect(AVEffect &&add, AVEffectList::OffsetValueT offsetX, AVEffectList::OffsetValueT offsetY, AVEffectList::OffsetValueT offsetZ)
        {
            return AddImpl(std::move(add), offsetX, offsetY, offsetZ, avEffects);
        }

        SenseComponent::AVEffectList::ID SenseComponent::AddAVEffect(AVEffectList::ID id, AVEffect &&add, AVEffectList::OffsetValueT offsetX, AVEffectList::OffsetValueT offsetY, AVEffectList::OffsetValueT offsetZ)
        {
            return AddImpl(id, std::move(add), offsetX, offsetY, offsetZ, avEffects);
        }

        void SenseComponent::RemoveAVEffect(const AVEffect &remove)
        {
            RemoveByObjectImpl(remove, avEffects);
        }

        void SenseComponent::RemoveAVEffect(AVEffectList::ID id)
        {
            RemoveByIDImpl(id, avEffects);
        }

        SenseComponent::AVEffectHandle* SenseComponent::FindAVEffect(AVEffectList::ID id)
        {
            return FindImpl(id, avEffects);
        }

        const SenseComponent::AVEffectHandle* SenseComponent::FindAVEffect(AVEffectList::ID id) const
        {
            return FindImpl(id, avEffects);
        }

        SenseComponent::ModulatorPack* SenseComponent::AddModulatorPack(const Name &name)
        {
            auto found = modulatorPacks.find(name);
            if (found != modulatorPacks.end())
                return &found->second;

            return &modulatorPacks.emplace(name, ModulatorPack()).first->second;
        }

        void SenseComponent::RemoveModulatorPack(const Name &name)
        {
            modulatorPacks.erase(name);
        }

        SenseComponent::ModulatorPack* SenseComponent::FindModulatorPack(const Name &name)
        {
            auto found = modulatorPacks.find(name);
            if (found != modulatorPacks.end())
                return &found->second;

            return &found->second;
        }

        const SenseComponent::ModulatorPack* SenseComponent::FindModulatorPack(const Name &name) const
        {
            auto found = modulatorPacks.find(name);
            if (found != modulatorPacks.end())
                return &found->second;

            return &found->second;
        }

        void SenseComponent::StartModulatorPack(const Name &name)
        {
            auto found = modulatorPacks.find(name);
            if (found == modulatorPacks.end())
            {
                ErrorHandler::Log("A modulator in a sense component was attempted to be started, but was not found. Discarding.",
                    ErrorHandler::Severity::ERROR_LOW,
                    ErrorHandler::NameValueVector{ NameValuePair("Modulator Name", name), NameValuePair("Entity ID", *GetOwnerEntity()) });
                return;
            }

            found->second.StartAll(*this);
        }

        void SenseComponent::StopModulatorPack(const Name &name)
        {
            auto found = modulatorPacks.find(name);
            if (found == modulatorPacks.end())
                return;

            found->second.StopAll();
        }

        void SenseComponent::Show(bool set)
        {
            show = set;
            SyncObjects();
        }

        void SenseComponent::Hide()
        {
            Show(false);
        }

        bool SenseComponent::IsShown() const
        {
            return show;
        }

        void SenseComponent::SyncObjects()
        {
            if (show)
            {
                for (auto &loop : sprites)
                {
                    GetCurrentRenderFragments()->Add(*loop.second);
                    loop.second->Enable(true);
                }

                for (auto &loop : sounds)
                {
                    loop.second->Play();
                    loop.second->Enable();
                }

                for (auto &loop : avEffects)
                {
                    loop.second->Start();
                }
            }
            else
            {
                for (auto &loop : sprites)
                {
                    GetCurrentRenderFragments()->Remove(*loop.second);
                    loop.second->Disable();
                }

                for (auto &loop : sounds)
                {
                    loop.second->Stop();
                    loop.second->Disable();
                }

                for (auto &loop : avEffects)
                {
                    loop.second->Stop();
                }
            }
        }

        void SenseComponent::SetPosition(const PositionT &set)
        {
            position = set;
            InformAllSpritesPositionChanged();
        }

        void SenseComponent::SetX(PositionT::ValueT set)
        {
            position.SetX(set);
            InformAllSpritesPositionChanged();
        }

        void SenseComponent::SetY(PositionT::ValueT set)
        {
            position.SetY(set);
            InformAllSpritesPositionChanged();
        }

        void SenseComponent::SetZ(PositionT::ValueT set)
        {
            position.SetZ(set);
            InformAllSpritesPositionChanged();
        }

        void SenseComponent::InformAllSpritesPositionChanged()
        {
            for (auto &loop : sprites)
                loop.second.InformOwnerPositionChange();
        }

        const SenseComponent::PositionT& SenseComponent::GetPosition() const
        {
            return position;
        }

        SenseComponent::PositionT::ValueT SenseComponent::GetX() const
        {
            return position.GetX();
        }

        SenseComponent::PositionT::ValueT SenseComponent::GetY() const
        {
            return position.GetY();
        }

        SenseComponent::PositionT::ValueT SenseComponent::GetZ() const
        {
            return position.GetZ();
        }
    }
}