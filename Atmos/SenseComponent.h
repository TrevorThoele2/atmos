#pragma once

#include <memory>
#include <unordered_map>
#include "EntityComponent.h"

#include "Sprite.h"
#include "Sound.h"
#include "AVEffect.h"

#include "OffsetManagedList.h"

namespace Atmos
{
    namespace Ent
    {
        class SenseComponent : public Component<SenseComponent>
        {
        public:
            typedef Position3D PositionT;

            // Lists
            typedef OffsetManagedList<Sprite> SpriteList;
            typedef OffsetManagedList<Sound> SoundList;
            typedef OffsetManagedList<AVEffect> AVEffectList;

            // Handles
            typedef SpriteList::HandleT SpriteHandle;
            typedef SoundList::HandleT SoundHandle;
            typedef AVEffectList::HandleT AVEffectHandle;

            // Modulators
            typedef Modulator::Modulator<SpriteHandle> SpriteModulator;
            typedef Modulator::Modulator<SoundHandle> SoundModulator;
            typedef Modulator::Modulator<AVEffectHandle> AVEffectModulator;

            enum class ModulatorEntryType
            {
                SPRITE,
                SOUND,
                AV_EFFECT
            };

            class ModulatorEntry
            {
            public:
                // The ID that is used to find the object it's going to modulate
                typedef size_t ObjectID;
                typedef ModulatorEntryType Type;
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            private:
                ObjectID id;
                Type type;
                Modulator::GeneratorRoute generatorRoute;
                Modulator::Observer modulatorObserver;
            public:
                ModulatorEntry();
                ModulatorEntry(ObjectID id, Type type, const Modulator::GeneratorRoute &generatorRoute);
                ModulatorEntry(ObjectID id, Type type, Modulator::GeneratorRoute &&generatorRoute);
                ModulatorEntry(const ModulatorEntry &arg) = default;
                ModulatorEntry(ModulatorEntry &&arg);
                ModulatorEntry& operator=(const ModulatorEntry &arg) = default;
                ModulatorEntry& operator=(ModulatorEntry &&arg);
                bool operator==(const ModulatorEntry &arg) const;
                bool operator!=(const ModulatorEntry &arg) const;

                Modulator::Observer Start(SenseComponent &component);
                void Stop();
                bool IsWorking() const;
                TimeValue GetSumTimeTaken() const;
            };

            class ModulatorPack
            {
            public:
                typedef size_t EntryID;
            private:
                INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
                INSCRIPTION_ACCESS;
            private:
                std::vector<ModulatorEntry> entries;
            public:
                ModulatorPack() = default;
                ModulatorPack(const ModulatorPack &arg) = default;
                ModulatorPack(ModulatorPack &&arg);
                ModulatorPack& operator=(const ModulatorPack &arg) = default;
                ModulatorPack& operator=(ModulatorPack &&arg);
                bool operator==(const ModulatorPack &arg) const;
                bool operator!=(const ModulatorPack &arg) const;

                void StartAll(SenseComponent &component);
                void StopAll();
                bool IsWorking() const;
                TimeValue GetTimeTaken() const;

                ModulatorEntry* Add(ModulatorEntry::ObjectID id, ModulatorEntryType type);
                ModulatorEntry* Add(ModulatorEntry::ObjectID id, ModulatorEntryType type, const Modulator::GeneratorRoute &generatorRoute);
                ModulatorEntry* Add(ModulatorEntry::ObjectID id, ModulatorEntryType type, Modulator::GeneratorRoute &&generatorRoute);
                ModulatorEntry* Add(const ModulatorEntry &add);
                ModulatorEntry* Add(ModulatorEntry &&add);
                void Remove(EntryID remove);
            };

            typedef std::unordered_map<Name, ModulatorPack> ModulatorPackMap;
        private:
            template<class T>
            struct ModulatorTraits;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            template<class T>
            typename OffsetManagedList<T>::ID AddImpl(OffsetManagedList<T> &cont);
            template<class ID, class T>
            typename OffsetManagedList<T>::ID AddImpl(ID id, OffsetManagedList<T> &cont);
            template<class T>
            typename OffsetManagedList<T>::ID AddImpl(const T &t, OffsetManagedList<T> &cont);
            template<class T, class OffsetValueT>
            typename OffsetManagedList<T>::ID AddImpl(T &&t, OffsetValueT offsetX, OffsetValueT offsetY, OffsetValueT offsetZ, OffsetManagedList<T> &cont);
            template<class ID, class T, class OffsetValueT>
            typename OffsetManagedList<T>::ID AddImpl(ID id, T &&t, OffsetValueT offsetX, OffsetValueT offsetY, OffsetValueT offsetZ, OffsetManagedList<T> &cont);
            template<class T, class Cont>
            void RemoveByObjectImpl(const T &t, Cont &cont);
            template<class ID, class Cont>
            void RemoveByIDImpl(ID id, Cont &cont);
            template<class T, class Cont, class Func>
            void RemoveByObjectImpl(const T &t, Cont &cont, Func func);
            template<class ID, class Cont, class Func>
            void RemoveByIDImpl(ID id, Cont &cont, Func func);
            template<class T>
            typename OffsetManagedList<T>::HandleT* FindImpl(typename OffsetManagedList<T>::ID id, OffsetManagedList<T> &cont);
            template<class T>
            typename const OffsetManagedList<T>::HandleT* FindImpl(typename OffsetManagedList<T>::ID id, const OffsetManagedList<T> &cont) const;
        public:
            PositionT position;

            SpriteList sprites;
            SoundList sounds;
            AVEffectList avEffects;

            ModulatorPackMap modulatorPacks;

            bool show;

            SenseComponent();
            SenseComponent(const SenseComponent &arg);
            SenseComponent& operator=(const SenseComponent &arg);
            SenseComponent(SenseComponent &&arg);
            SenseComponent& operator=(SenseComponent &&arg);

            bool operator==(const SenseComponent &arg) const;
            bool operator!=(const SenseComponent &arg) const;

            SpriteList::ID AddSprite();
            SpriteList::ID AddSprite(SpriteList::ID id);
            SpriteList::ID AddSprite(const Sprite &add);
            SpriteList::ID AddSprite(Sprite &&add, SpriteList::OffsetValueT offsetX, SpriteList::OffsetValueT offsetY, SpriteList::OffsetValueT offsetZ);
            SpriteList::ID AddSprite(SpriteList::ID id, Sprite &&add, SpriteList::OffsetValueT offsetX, SpriteList::OffsetValueT offsetY, SpriteList::OffsetValueT offsetZ);
            void RemoveSprite(const Sprite &remove);
            void RemoveSprite(SpriteList::ID id);
            SpriteHandle* FindSprite(SpriteList::ID id);
            const SpriteHandle* FindSprite(SpriteList::ID id) const;

            SoundList::ID AddSound();
            SoundList::ID AddSound(SoundList::ID id);
            SoundList::ID AddSound(const Sound &add);
            SoundList::ID AddSound(Sound &&add, SoundList::OffsetValueT offsetX, SoundList::OffsetValueT offsetY, SoundList::OffsetValueT offsetZ);
            SoundList::ID AddSound(SoundList::ID id, Sound &&add, SoundList::OffsetValueT offsetX, SoundList::OffsetValueT offsetY, SoundList::OffsetValueT offsetZ);
            void RemoveSound(const Sound &remove);
            void RemoveSound(SoundList::ID id);
            SoundHandle* FindSound(SoundList::ID id);
            const SoundHandle* FindSound(SoundList::ID id) const;

            AVEffectList::ID AddAVEffect();
            AVEffectList::ID AddAVEffect(AVEffectList::ID id);
            AVEffectList::ID AddAVEffect(const AVEffect &add);
            AVEffectList::ID AddAVEffect(AVEffect &&add, AVEffectList::OffsetValueT offsetX, AVEffectList::OffsetValueT offsetY, AVEffectList::OffsetValueT offsetZ);
            AVEffectList::ID AddAVEffect(AVEffectList::ID id, AVEffect &&add, AVEffectList::OffsetValueT offsetX, AVEffectList::OffsetValueT offsetY, AVEffectList::OffsetValueT offsetZ);
            void RemoveAVEffect(const AVEffect &remove);
            void RemoveAVEffect(AVEffectList::ID id);
            AVEffectHandle* FindAVEffect(AVEffectList::ID id);
            const AVEffectHandle* FindAVEffect(AVEffectList::ID id) const;

            ModulatorPack* AddModulatorPack(const Name &name);
            void RemoveModulatorPack(const Name &name);
            ModulatorPack* FindModulatorPack(const Name &name);
            const ModulatorPack* FindModulatorPack(const Name &name) const;
            void StartModulatorPack(const Name &name);
            void StopModulatorPack(const Name &name);

            void Show(bool set = true);
            void Hide();
            bool IsShown() const;
            // If show is true, then this will add the objects to the render fragment handler
            // If show is false, then this will remove the objects from the render fragment handler
            // It will also show/hide all of the individual objects
            void SyncObjects();

            void SetPosition(const PositionT &set);
            void SetX(PositionT::ValueT set);
            void SetY(PositionT::ValueT set);
            void SetZ(PositionT::ValueT set);
            void InformAllSpritesPositionChanged();
            const PositionT& GetPosition() const;
            PositionT::ValueT GetX() const;
            PositionT::ValueT GetY() const;
            PositionT::ValueT GetZ() const;
        };

        template<class T>
        typename OffsetManagedList<T>::ID SenseComponent::AddImpl(OffsetManagedList<T> &cont)
        {
            return cont.Add(T()).second->first;
        }

        template<class ID, class T>
        typename OffsetManagedList<T>::ID SenseComponent::AddImpl(ID id, OffsetManagedList<T> &cont)
        {
            return cont.Add(id, T()).second->first;
        }

        template<class T>
        typename OffsetManagedList<T>::ID SenseComponent::AddImpl(const T &t, OffsetManagedList<T> &cont)
        {
            return cont.Add(t).second->first;
        }

        template<class T, class OffsetValueT>
        typename OffsetManagedList<T>::ID SenseComponent::AddImpl(T &&t, OffsetValueT offsetX, OffsetValueT offsetY, OffsetValueT offsetZ, OffsetManagedList<T> &cont)
        {
            t.SetPosition(position);
            return cont.Add(std::move(t), offsetX, offsetY, offsetZ).second->first;
        }

        template<class ID, class T, class OffsetValueT>
        typename OffsetManagedList<T>::ID SenseComponent::AddImpl(ID id, T &&t, OffsetValueT offsetX, OffsetValueT offsetY, OffsetValueT offsetZ, OffsetManagedList<T> &cont)
        {
            t.SetPosition(position);
            return cont.Add(id, std::move(t), offsetX, offsetY, offsetZ).second->first;
        }

        template<class T, class Cont>
        void SenseComponent::RemoveByObjectImpl(const T &t, Cont &cont)
        {
            for (auto loop = cont.begin(); loop != cont.end();)
            {
                if (&*loop->second == &t)
                {
                    cont.Remove(loop);
                    return;
                }
                else
                    ++loop;
            }
        }

        template<class ID, class Cont>
        void SenseComponent::RemoveByIDImpl(ID id, Cont &cont)
        {
            auto found = cont.Find(id);
            if (found == cont.end())
                return;

            cont.Remove(found);
        }

        template<class T, class Cont, class Func>
        void SenseComponent::RemoveByObjectImpl(const T &t, Cont &cont, Func func)
        {
            for (auto loop = cont.begin(); loop != cont.end();)
            {
                if (&*loop->second == &t)
                {
                    func(*loop->second);
                    cont.Remove(loop);
                    return;
                }
                else
                    ++loop;
            }
        }

        template<class ID, class Cont, class Func>
        void SenseComponent::RemoveByIDImpl(ID id, Cont &cont, Func func)
        {
            auto found = cont.Find(id);
            if (found == cont.end())
                return;

            cont.Remove(found);
        }

        template<class T>
        typename OffsetManagedList<T>::HandleT* SenseComponent::FindImpl(typename OffsetManagedList<T>::ID id, OffsetManagedList<T> &cont)
        {
            auto found = cont.Find(id);
            if (found == cont.end())
                return nullptr;

            return &found->second;
        }

        template<class T>
        typename const OffsetManagedList<T>::HandleT* SenseComponent::FindImpl(typename OffsetManagedList<T>::ID id, const OffsetManagedList<T> &cont) const
        {
            auto found = cont.Find(id);
            if (found == cont.end())
                return nullptr;

            return &found->second;
        }

        ENTITY_COMPONENT_MAP_DECLARE("Sense", SenseComponent)
    }
}