
/*
#pragma once

#include "Item.h"

#include "Serialization.h"

namespace Atmos
{
    class ConsumableBase : public Item
    {
    public:
        class Stack : public Item::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef ConsumableBase WrappedT;
        private:
            virtual void ConsumeImpl(Entity entity) = 0;
        protected:
            WrappedT& RetrieveItem() override;
        public:
            Stack(WrappedT &item, const Count &count);
            Stack(const Stack &arg);
            Stack& operator=(const Stack &arg);

            virtual ~Stack() = 0 {}
            virtual Stack* Clone() const = 0;
            const WrappedT& GetItem() const override;

            void Consume(Entity entity);
            virtual bool CanConsume(Entity entity) const = 0;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(ConsumableBase, private);
    public:
        ConsumableBase(const std::string &name, const std::string &description, Price buyPrice, Price sellPrice);
        ConsumableBase(const ConsumableBase &arg);
        ConsumableBase& operator=(const ConsumableBase &arg);
        virtual ~ConsumableBase() = 0 {}
    };
}

namespace inscription
{
    template<>
    class Inscripter<::Atmos::ConsumableBase::Stack> : public InscripterBase<::Atmos::ConsumableBase::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };

    template<>
    class Inscripter<::Atmos::ConsumableBase> : public InscripterBase<::Atmos::ConsumableBase>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}
*/