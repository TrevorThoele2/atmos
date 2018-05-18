
/*
#pragma once

#include "ConsumableBase.h"
#include "StatusEffect.h"

#include "Serialization.h"

namespace Atmos
{
    class Consumable : public ConsumableBase
    {
    public:
        class Stack : public ConsumableBase::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef Consumable WrappedT;
        private:
            void ConsumeImpl(Entity entity) override;
        protected:
            WrappedT& RetrieveItem() override;
        public:
            Stack(WrappedT &item, const Count &count);
            Stack(const Stack &arg);
            Stack& operator=(const Stack &arg);

            Stack* Clone() const override;
            const WrappedT& GetItem() const override;

            bool CanConsume(Entity entity) const override;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Consumable, private);
    private:
        StatusEffect *statusEffect;
    public:
        Consumable(const Name &name, const Description &description, Price buyPrice, Price sellPrice, StatusEffect &statusEffect);
        Consumable(const Consumable &arg);
        Consumable& operator=(const Consumable &arg);
        Stack* CreateStack(Stack::SizeT count) override;

        void SetStatusEffect(StatusEffect &set);
        StatusEffect& GetStatusEffect();
    };
}

namespace inscription
{
    template<>
    class Inscripter<::Atmos::Consumable::Stack> : public InscripterBase<::Atmos::Consumable::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };

    template<>
    class Inscripter<::Atmos::Consumable> : public InscripterBase<::Atmos::Consumable>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };
}
*/