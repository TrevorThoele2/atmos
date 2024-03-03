#pragma once

#include "EntityComponent.h"
#include "FieldID.h"
#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class CharacterComponent : public Component<CharacterComponent>
        {
        public:
            enum Type
            {
                PLAYER,
                NPC,
                // A monster is also an NPC
                MONSTER
            };

            class ImplBase
            {
            public:
                virtual ~ImplBase() = 0 {}
                virtual ImplBase* Clone() const = 0;
            };

            class ImplNone : public ImplBase
            {
            public:
                ImplNone* Clone() const override final;
            };

            class ImplMonster : public ImplBase
            {
            public:
                FieldID fieldID;
                ImplMonster() = default;
                ImplMonster(const FieldID &fieldID);
                ImplMonster* Clone() const override final;
            };

            template<Type t>
            struct Traits;

            template<>
            struct Traits<Type::PLAYER>
            {
                typedef ImplNone ImplT;
            };

            template<>
            struct Traits<Type::NPC>
            {
                typedef ImplNone ImplT;
            };

            template<>
            struct Traits<Type::MONSTER>
            {
                typedef ImplMonster ImplT;
            };
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Type type;
            
            typedef std::unique_ptr<ImplBase> ImplPtr;
            ImplPtr impl;

            CharacterComponent(Type type = Type::PLAYER);
            CharacterComponent(const CharacterComponent &arg);
            CharacterComponent(CharacterComponent &&arg);
            CharacterComponent& operator=(const CharacterComponent &arg);
            CharacterComponent& operator=(CharacterComponent &&arg);

            void SetPlayer();
            void SetNPC();
            void SetMonster();
            void SetType(Type set);

            template<Type t>
            typename Traits<t>::ImplT* GetImpl();

            bool IsType(Type check) const;
            template<Type check>
            bool IsType() const;
            bool IsPlayer() const;
            // A monster is also an NPC
            bool IsNPC() const;
            bool IsMonster() const;
        };

        template<CharacterComponent::Type t>
        typename CharacterComponent::Traits<t>::ImplT* CharacterComponent::GetImpl()
        {
            return static_cast<typename Traits<t>::ImplT*>(impl.get());
        }

        template<CharacterComponent::Type check>
        bool CharacterComponent::IsType() const
        {
            return type == check;
        }

        ENTITY_COMPONENT_MAP_DECLARE("Character", CharacterComponent)
    }
}