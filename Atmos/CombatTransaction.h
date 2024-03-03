
#pragma once

#include "FixedPoint.h"
#include "Element.h"
#include "Name.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent { class CombatComponent; }

    class CombatTransactionGenerator;
    class CombatTransaction
    {
    public:
        typedef FixedPoint64 Amount;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Amount amount;
        Name positiveStatName;
        Element element;
        CombatTransaction(Amount amount, const Name &attackStatName, const Element &element);
    public:
        bool operator==(const CombatTransaction &arg) const;
        bool operator!=(const CombatTransaction &arg) const;
        // Collide against the defense of the target
        void Collide(Ent::CombatComponent &target) const;
        friend CombatTransactionGenerator;
    };

    class CombatTransactionGenerator
    {
    
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Name resourceName;
        Name statName;
        Element element;
        Name proficiencyName;
    public:
        CombatTransactionGenerator() = default;
        // Power is represented by a percentage that can go over 100%
        CombatTransactionGenerator(const Name &resourceName, const Name &statName, const Element &element, const Name &proficiencyName);
        bool operator==(const CombatTransactionGenerator &arg) const;
        bool operator!=(const CombatTransactionGenerator &arg) const;

        CombatTransaction Generate(const CombatTransaction::Amount &baseAmount, const Ent::CombatComponent &source) const;

        void SetResourceName(const Name &set);
        void SetStatName(const Name &set);
        void SetElement(Element set);
        void SetProficiencyName(const Name &set);

        const Name& GetResourceName() const;
        const Name& GetStatName() const;
        Element GetElement() const;
        const Name& GetProficiencyName() const;
    };
}