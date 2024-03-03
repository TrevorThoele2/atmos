
#include "CombatTransaction.h"

#include "CombatComponent.h"
#include "CapCombatValue.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CombatTransaction)
    {
        scribe(amount);
        scribe(positiveStatName);
        scribe(element);
    }

    CombatTransaction::CombatTransaction(Amount amount, const Name &positiveStatName, const Element &element) : amount(amount), positiveStatName(positiveStatName), element(element)
    {}

    bool CombatTransaction::operator==(const CombatTransaction &arg) const
    {
        return amount == arg.amount && positiveStatName == arg.positiveStatName && element == arg.element;
    }

    bool CombatTransaction::operator!=(const CombatTransaction &arg) const
    {
        return !(*this == arg);
    }

    void CombatTransaction::Collide(Ent::CombatComponent &target) const
    {
        if (target.IsCorpse())
            return;
        /*
        Defense acts as the same as above
            Each defense point counts as 1/2
        */

        typedef Amount::Split Split;
        
        Amount defense(CalculateValueCap(Split(target.GetDefense(), 0), { Split(*target.stats.GetValue(*DataStandard<StatAttributeTable>::GetOpposing(positiveStatName)), 0) }));

        // Apply elemental multiplier to attack
        defense *= element.GetAdvantageAgainst(target.GetElement());
        // Apply elemental resistances
        defense += Split(*target.stats.GetValue(*DataStandard<StatAttributeTable>::GetOpposing(element.Get())), 0);
        // Halve the defense
        defense *= Split(0, 5);

        /*
        Damage after defense
        if(att >= def)
            damage = floor(attack * (1 - (((def / 2) / att) * 0.5)))
        else
            damage = floor(attack * ((att / (def / 2)) * .5))
        */

        Amount damage;
        const Split one(1, 0);
        const Split doubled(2, 0);
        const Split halved(0, 5);
        damage = (amount >= defense) ? Floor(amount * (one - (((defense / doubled) / amount) * halved))) : Floor(amount * ((amount / (defense / doubled)) * halved));

        target.resources.AddModifier(ResourceAttributeTable::Modifier(GlobalContext<ResourceAttributeTable>::health, ResourceAttributeTable::ValueT(Round(damage)), OperatorSelector::SUBTRACT, ResourceAttributeTable::Modifier::Type::VALUE));
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CombatTransactionGenerator)
    {
        scribe(resourceName);
        scribe(statName);
        scribe(element);
        scribe(proficiencyName);
    }

    CombatTransactionGenerator::CombatTransactionGenerator(const Name &resourceName, const Name &statName, const Element &element, const Name &proficiencyName) : resourceName(resourceName), statName(statName), element(element), proficiencyName(proficiencyName)
    {}

    bool CombatTransactionGenerator::operator==(const CombatTransactionGenerator &arg) const
    {
        return resourceName == arg.resourceName && statName == arg.statName && element == arg.element && proficiencyName == arg.proficiencyName;
    }

    bool CombatTransactionGenerator::operator!=(const CombatTransactionGenerator &arg) const
    {
        return !(*this == arg);
    }

    CombatTransaction CombatTransactionGenerator::Generate(const CombatTransaction::Amount &baseAmount, const Ent::CombatComponent &source) const
    {
        /*
        Each attack stat gives +X to an attack up to 50% of the base damage of the weapon equipped.
        Each stat point above the base that gives +1/2X to an attack up to 150% of the base damage of the weapon equipped..
        Each stat point above that is +1/3X.
        X is dependent upon the % of power that the pattern piece gives.
        */

        typedef CombatTransaction::Amount Amount;
        typedef Amount::Split Split;

        // Apply stat and proficiency
        Amount amount(CalculateValueCap(baseAmount, { Split(*source.stats.GetValue(statName), 0), source.proficiencies.GetLevel(proficiencyName) / Split(2, 0) }));

        // Apply elemental multiplier to attack
        amount *= element.GetSameMultiplier(source.GetElement());
        // Apply elemental power
        amount += Split(*source.stats.GetValue(*GlobalContext<Element>::GetStatName(element.Get())), 0);

        return CombatTransaction(amount, statName, element);
    }

    void CombatTransactionGenerator::SetResourceName(const Name &set)
    {
        resourceName = set;
    }

    void CombatTransactionGenerator::SetStatName(const Name &set)
    {
        statName = set;
    }

    void CombatTransactionGenerator::SetElement(Element set)
    {
        element = set;
    }

    void CombatTransactionGenerator::SetProficiencyName(const Name &set)
    {
        proficiencyName = set;
    }

    const Name& CombatTransactionGenerator::GetResourceName() const
    {
        return resourceName;
    }

    const Name& CombatTransactionGenerator::GetStatName() const
    {
        return statName;
    }

    Element CombatTransactionGenerator::GetElement() const
    {
        return element;
    }

    const Name& CombatTransactionGenerator::GetProficiencyName() const
    {
        return proficiencyName;
    }
}