
#pragma once

#include "CombatComponent.h"
#include "ObjectReference.h"

#include "FixedPoint.h"
#include "Name.h"

#include "Serialization.h"

namespace Atmos
{
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
        CombatTransaction(Amount amount, const Name &attackStatName);
    public:
        bool operator==(const CombatTransaction &arg) const;
        bool operator!=(const CombatTransaction &arg) const;
        // Collide against the defense of the target
        void Collide(TypedObjectReference<Ent::nCombatComponent> target) const;
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
        Name proficiencyName;
    public:
        CombatTransactionGenerator() = default;
        // Power is represented by a percentage that can go over 100%
        CombatTransactionGenerator(const Name &resourceName, const Name &statName, const Name &proficiencyName);
        bool operator==(const CombatTransactionGenerator &arg) const;
        bool operator!=(const CombatTransactionGenerator &arg) const;

        CombatTransaction Generate(const CombatTransaction::Amount &baseAmount, TypedObjectReference<Ent::nCombatComponent> source) const;

        void SetResourceName(const Name &set);
        void SetStatName(const Name &set);
        void SetProficiencyName(const Name &set);

        const Name& GetResourceName() const;
        const Name& GetStatName() const;
        const Name& GetProficiencyName() const;
    };
}