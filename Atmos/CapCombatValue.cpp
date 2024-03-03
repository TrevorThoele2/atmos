
#include "CapCombatValue.h"

namespace Atmos
{
    FixedPoint64 CalculateValueCap(FixedPoint64 power, std::vector<FixedPoint64> stats)
    {
        /*
        Each attack stat gives +X to an attack up to 50% of the base damage of the weapon equipped.
        Each stat point above the base that gives +1/2X to an attack up to 150% of the base damage of the weapon equipped..
        Each stat point above that is +1/3X.
        X is dependent upon the % of power that the pattern piece gives.
        */

        typedef FixedPoint64 Amount;
        typedef Amount::Split Split;
        const Amount smallCutoff(Split(1, Split::AdjustF(5, Amount::GetDefaultRadixPoint())));

        Amount ret(power);

        for (auto& loop : stats)
        {
            if (loop > power * smallCutoff)
            {
                // Take care up to 100%
                ret *= Split(2, 0);
                loop -= power;
                // Take care up to 150%
                ret += power * Split(0, Split::AdjustF(25, Amount::GetDefaultRadixPoint()));
                loop -= power * Split(0, Split::AdjustF(5, Amount::GetDefaultRadixPoint()));
                // Rest
                ret += loop / Split(3, 0);
            }
            else if (loop > power)
            {
                // Take care up to 100%
                ret *= Split(2, 0);
                loop -= power;
                // Take care up to 150%
                ret += loop / Split(2, 0);
            }
            else
                ret += loop;
        }

        return ret;
    }
}