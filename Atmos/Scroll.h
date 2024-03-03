
/*
#pragma once

#include "ConsumableBase.h"
#include "Spell.h"

#include "Serialization.h"

namespace Atmos
{
    class Scroll : public ConsumableBase
    {
    public:
        class Stack : public ConsumableBase::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef Scroll WrappedT;
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

            bool CanConsume(Entity entity) const;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Scroll, private);
    private:
        const Spell *spell;
    public:
        Scroll(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const Spell &spell);
        Scroll(const Scroll &arg);
        Scroll& operator=(const Scroll &arg);
        Stack* CreateStack(Stack::SizeT count) override;

        void SetSpell(const Spell &set);
        const Spell& GetSpell() const;
    };
}

namespace inscription
{
    template<>
    class Inscripter<::Atmos::Scroll::Stack> : public InscripterBase<::Atmos::Scroll::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };

    template<>
    class Inscripter<::Atmos::Scroll> : public InscripterBase<::Atmos::Scroll>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };
}
*/