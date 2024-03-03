
#include "EquippableAspect.h"

#include "CombatComponent.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(EquippableAspect)
    {
        scribe(equipRegion);
        if (scribe.IsOutput())
        {
            bool hasAdditional = HasAdditional();
            // Save if has additional
            scribe.Save(hasAdditional);
            // Is has additional...
            if (hasAdditional)
            {
                AdditionalType type = additional->GetType();
                // Save type
                scribe.Save(type);
                // Save additional
                switch (type)
                {
                case AdditionalType::AMMO:
                {
                    AmmoAdditional *ammo = static_cast<AmmoAdditional*>(additional.get());
                    break;
                }
                case AdditionalType::ATTACK:
                {
                    AttackAdditional *attack = static_cast<AttackAdditional*>(additional.get());
                    break;
                }
                }
            }
        }
        else
        {
            bool hasAdditional = false;
            // Load if has additional
            scribe.Load(hasAdditional);
            // Is has additional...
            if (hasAdditional)
            {
                AdditionalType type = AdditionalType::AMMO;
                // Load type
                scribe.Load(type);
                // Load additional
                switch (type)
                {
                case AdditionalType::AMMO:
                {
                    additional.reset(new AmmoAdditional());
                    break;
                }
                case AdditionalType::ATTACK:
                {
                    additional.reset(new AttackAdditional());
                    break;
                }
                }
            }
            else
                additional.reset();
        }
    }

    bool EquippableAspect::AmmoAdditional::operator==(const AmmoAdditional &arg) const
    {
        return ammoType == arg.ammoType;
    }

    bool EquippableAspect::AmmoAdditional::operator!=(const AmmoAdditional &arg) const
    {
        return !(*this == arg);
    }

    EquippableAspect::AmmoAdditional* EquippableAspect::AmmoAdditional::Clone() const
    {
        return new AmmoAdditional(*this);
    }

    EquippableAspect::AdditionalType EquippableAspect::AmmoAdditional::GetType() const
    {
        return AdditionalType::AMMO;
    }

    bool EquippableAspect::AttackAdditional::UsesAmmoAdditional::operator==(const UsesAmmoAdditional &arg) const
    {
        return this == &arg;
    }

    bool EquippableAspect::AttackAdditional::UsesAmmoAdditional::operator!=(const UsesAmmoAdditional &arg) const
    {
        return !(*this == arg);
    }

    EquippableAspect::AttackAdditional::AttackAdditional(AttackAdditional &&arg) : power(std::move(arg.power)), pattern(std::move(arg.pattern)), range(std::move(arg.range)), allowedClasses(std::move(arg.allowedClasses)), proficiencyName(std::move(arg.proficiencyName)), usesAmmo(std::move(arg.usesAmmo))
    {}

    EquippableAspect::AttackAdditional& EquippableAspect::AttackAdditional::operator=(AttackAdditional &&arg)
    {
        power = std::move(arg.power);
        pattern = std::move(arg.pattern);
        range = std::move(arg.range);
        allowedClasses = std::move(arg.allowedClasses);
        proficiencyName = std::move(arg.proficiencyName);
        usesAmmo = std::move(arg.usesAmmo);
        return *this;
    }

    bool EquippableAspect::AttackAdditional::operator==(const AttackAdditional &arg) const
    {
        return power == arg.power && pattern == arg.pattern && range == arg.range && allowedClasses == arg.allowedClasses && proficiencyName == arg.proficiencyName && usesAmmo == arg.usesAmmo;
    }

    bool EquippableAspect::AttackAdditional::operator!=(const AttackAdditional &arg) const
    {
        return !(*this == arg);
    }

    EquippableAspect::AttackAdditional* EquippableAspect::AttackAdditional::Clone() const
    {
        return new AttackAdditional(*this);
    }

    EquippableAspect::AdditionalType EquippableAspect::AttackAdditional::GetType() const
    {
        return AdditionalType::ATTACK;
    }

    void EquippableAspect::AttackAdditional::SignalUsesAmmo()
    {
        usesAmmo.Set(UsesAmmoAdditional());
    }

    bool EquippableAspect::AttackAdditional::IsUsesAmmo() const
    {
        return usesAmmo.IsValid();
    }

    void EquippableAspect::AttackAdditional::ActivatePattern(Ent::CombatComponent &source, const GridPosition &pos)
    {
        source.proficiencies.AttemptIncrement(proficiencyName, source);
    }

    bool EquippableAspect::ShardAdditional::operator==(const ShardAdditional &arg) const
    {
        return element == arg.element;
    }

    bool EquippableAspect::ShardAdditional::operator!=(const ShardAdditional &arg) const
    {
        return !(*this == arg);
    }

    EquippableAspect::ShardAdditional* EquippableAspect::ShardAdditional::Clone() const
    {
        return new ShardAdditional(*this);
    }

    EquippableAspect::AdditionalType EquippableAspect::ShardAdditional::GetType() const
    {
        return AdditionalType::SHARD;
    }

    EquippableAspect::EquippableAspect(const EquippableAspect &arg) : equipRegion(arg.equipRegion), defense(arg.defense), additional(arg.additional->Clone())
    {}

    EquippableAspect& EquippableAspect::operator=(const EquippableAspect &arg)
    {
        equipRegion = arg.equipRegion;
        defense = arg.defense;
        additional.reset(arg.additional->Clone());
        return *this;
    }

    EquippableAspect::EquippableAspect(EquippableAspect &&arg) : equipRegion(std::move(arg.equipRegion)), defense(std::move(arg.defense)), additional(std::move(arg.additional))
    {}

    EquippableAspect& EquippableAspect::operator=(EquippableAspect &&arg)
    {
        equipRegion = std::move(arg.equipRegion);
        defense = std::move(arg.defense);
        additional = std::move(arg.additional);
        return *this;
    }

    bool EquippableAspect::operator==(const EquippableAspect &arg) const
    {
        return equipRegion == arg.equipRegion && defense == arg.defense && additional == arg.additional;
    }

    bool EquippableAspect::operator!=(const EquippableAspect &arg) const
    {
        return !(*this == arg);
    }

    void EquippableAspect::SignalAmmo()
    {
        additional.reset(new AmmoAdditional());
    }

    void EquippableAspect::SignalAttack()
    {
        additional.reset(new AttackAdditional());
    }

    void EquippableAspect::SignalShard()
    {
        additional.reset(new ShardAdditional());
    }

    EquippableAspect::AmmoAdditional* EquippableAspect::AsAmmo()
    {
        return static_cast<AmmoAdditional*>(additional.get());
    }

    const EquippableAspect::AmmoAdditional* EquippableAspect::AsAmmo() const
    {
        return static_cast<AmmoAdditional*>(additional.get());
    }

    EquippableAspect::AttackAdditional* EquippableAspect::AsAttack()
    {
        return static_cast<AttackAdditional*>(additional.get());
    }

    const EquippableAspect::AttackAdditional* EquippableAspect::AsAttack() const
    {
        return static_cast<AttackAdditional*>(additional.get());
    }

    EquippableAspect::ShardAdditional* EquippableAspect::AsShard()
    {
        return static_cast<ShardAdditional*>(additional.get());
    }

    const EquippableAspect::ShardAdditional* EquippableAspect::AsShard() const
    {
        return static_cast<ShardAdditional*>(additional.get());
    }

    bool EquippableAspect::IsAmmo() const
    {
        return HasAdditional() && additional->GetType() == AdditionalType::AMMO;
    }

    bool EquippableAspect::IsAttack() const
    {
        return HasAdditional() && additional->GetType() == AdditionalType::ATTACK;
    }

    bool EquippableAspect::IsShard() const
    {
        return HasAdditional() && additional->GetType() == AdditionalType::SHARD;
    }

    bool EquippableAspect::HasAdditional() const
    {
        return additional.get() != nullptr;
    }
}