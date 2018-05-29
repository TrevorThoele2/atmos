
#include "SenseComponent.h"
#include "Entity.h"

#include "Field.h"

#include "CurrentField.h"

#include "Logger.h"

#include <Inscription/Inscripter.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(SenseComponent)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(position);
                scribe.Save(sprites);

                scribe.Save(show);
            }
            else // INPUT
            {
                scribe.Load(position);
                scribe.Load(sprites);
                sprites.SetOwnerPosition(&position);

                scribe.Load(show);

                SyncObjects();
            }
        }

        SenseComponent::SenseComponent() : show(true)
        {}

        SenseComponent::SenseComponent(const SenseComponent &arg) : Component(arg), position(arg.position), sprites(arg.sprites, &position), sounds(arg.sounds, &position), avEffects(arg.avEffects, &position), show(arg.show)
        {
            SyncObjects();
        }

        SenseComponent& SenseComponent::operator=(const SenseComponent &arg)
        {
            Component::operator=(arg);
            position = arg.position;
            sprites = arg.sprites;
            sounds = arg.sounds;
            avEffects = arg.avEffects;
            show = arg.show;
            SyncObjects();
            return *this;
        }

        SenseComponent::SenseComponent(SenseComponent &&arg) : Component(std::move(arg)), position(std::move(arg.position)), sprites(std::move(arg.sprites), &position), sounds(std::move(arg.sounds), &position), avEffects(std::move(arg.avEffects), &position), show(std::move(arg.show))
        {
            SyncObjects();
        }

        SenseComponent& SenseComponent::operator=(SenseComponent &&arg)
        {
            Component::operator=(std::move(arg));
            position = std::move(arg.position);
            sprites = std::move(arg.sprites);
            sounds = std::move(arg.sounds);
            avEffects = std::move(arg.avEffects);
            show = arg.show;
            SyncObjects();
            return *this;
        }

        bool SenseComponent::operator==(const SenseComponent &arg) const
        {
            return position == arg.position && sprites == arg.sprites && sounds == arg.sounds && avEffects == arg.avEffects && show == arg.show;
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