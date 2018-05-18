
#include "CharacterComponent.h"

namespace Atmos
{
    namespace Ent
    {
        CharacterComponent::ImplNone* CharacterComponent::ImplNone::Clone() const
        {
            return new ImplNone(*this);
        }

        CharacterComponent::ImplMonster::ImplMonster(const FieldID &fieldID) : fieldID(fieldID)
        {}

        CharacterComponent::ImplMonster* CharacterComponent::ImplMonster::Clone() const
        {
            return new ImplMonster(*this);
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CharacterComponent)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(type);
                switch (type)
                {
                case Type::PLAYER:
                    break;
                case Type::NPC:
                    break;
                case Type::MONSTER:
                    scribe.Save(GetImpl<Type::MONSTER>()->fieldID);
                    break;
                }
            }
            else // INPUT
            {
                scribe.Load(type);
                switch (type)
                {
                case Type::PLAYER:
                    impl.reset(new Traits<Type::PLAYER>::ImplT());
                    break;
                case Type::NPC:
                    impl.reset(new Traits<Type::NPC>::ImplT());
                    break;
                case Type::MONSTER:
                {
                    FieldID fieldID;
                    scribe.Load(fieldID); 
                    impl.reset(new Traits<Type::MONSTER>::ImplT(fieldID));
                    break;
                }
                }
            }
        }

        CharacterComponent::CharacterComponent(Type type) : type(type)
        {}

        CharacterComponent::CharacterComponent(const CharacterComponent &arg) : type(arg.type), impl((arg.impl) ? arg.impl->Clone() : nullptr)
        {}

        CharacterComponent::CharacterComponent(CharacterComponent &&arg) : type(std::move(arg.type)), impl(std::move(arg.impl))
        {}

        CharacterComponent& CharacterComponent::operator=(const CharacterComponent &arg)
        {
            type = arg.type;
            impl.reset(arg.impl->Clone());
            return *this;
        }

        CharacterComponent& CharacterComponent::operator=(CharacterComponent &&arg)
        {
            type = std::move(arg.type);
            impl = std::move(arg.impl);
            return *this;
        }

        void CharacterComponent::SetPlayer()
        {
            type = PLAYER;
            impl.reset(new Traits<Type::PLAYER>::ImplT());
        }

        void CharacterComponent::SetNPC()
        {
            type = NPC;
            impl.reset(new Traits<Type::NPC>::ImplT());
        }

        void CharacterComponent::SetMonster()
        {
            type = MONSTER;
            impl.reset(new Traits<Type::MONSTER>::ImplT());
        }

        void CharacterComponent::SetType(Type set)
        {
            switch (set)
            {
            case Type::PLAYER:
                SetPlayer();
                break;
            case Type::NPC:
                SetNPC();
                break;
            case Type::MONSTER:
                SetMonster();
                break;
            }
        }

        bool CharacterComponent::IsType(Type check) const
        {
            return type == check;
        }

        bool CharacterComponent::IsPlayer() const
        {
            return type == PLAYER;
        }

        bool CharacterComponent::IsNPC() const
        {
            return type == NPC || type == MONSTER;
        }

        bool CharacterComponent::IsMonster() const
        {
            return type == MONSTER;
        }
    }
}