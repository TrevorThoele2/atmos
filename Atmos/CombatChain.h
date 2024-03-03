
#pragma once

#include "CombatTransaction.h"
#include "StatusEffect.h"
#include "RegistryObjectReference.h"

#include "Serialization.h"

namespace Atmos
{
    // Describes the sequence of events that a specific battle action will do
    class CombatChain
    {
    public:
        class Node
        {
        public:
            CombatTransactionGenerator transaction;
            RegistryObjectReference<StatusEffect> statusEffect;

            Node() = default;
            Node(const CombatTransactionGenerator &transaction, const RegistryObjectReference<StatusEffect> &statusEffect);
            Node(CombatTransactionGenerator &&transaction, RegistryObjectReference<StatusEffect> &&statusEffect);
            bool operator==(const Node &arg) const;
            bool operator!=(const Node &arg) const;
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        std::vector<Node> nodes;

        CombatChain() = default;
        CombatChain(const CombatChain &arg) = default;
        CombatChain(CombatChain &&arg);
        CombatChain& operator=(const CombatChain &arg) = default;
        CombatChain& operator=(CombatChain &&arg);
        bool operator==(const CombatChain &arg) const;
        bool operator!=(const CombatChain &arg) const;

        Node& AddNode();
        Node& AddNode(Node &&node);
    };
}