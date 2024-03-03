
#include "CombatChain.h"

#include <Inscription/Scribe.h>
#include <Inscription/Vector.h>

namespace Atmos
{
    CombatChain::Node::Node(const CombatTransactionGenerator &transaction, const RegistryObjectReference<StatusEffect> &statusEffect) : transaction(transaction), statusEffect(statusEffect)
    {}

    CombatChain::Node::Node(CombatTransactionGenerator &&transaction, RegistryObjectReference<StatusEffect> &&statusEffect) : transaction(std::move(transaction)), statusEffect(std::move(statusEffect))
    {}

    bool CombatChain::Node::operator==(const Node &arg) const
    {
        return transaction == arg.transaction && statusEffect == arg.statusEffect;
    }

    bool CombatChain::Node::operator!=(const Node &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CombatChain)
    {
        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(nodes.size());
            scribe.Save(size);

            for (auto &loop : nodes)
            {
                scribe.Save(loop.transaction);
                scribe.Save(loop.statusEffect);
            }
        }
        else // INPUT
        {
            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                CombatTransactionGenerator transaction;
                scribe.Load(transaction);

                RegistryObjectReference<StatusEffect> statusEffect;
                scribe.Load(statusEffect);

                nodes.push_back(Node(transaction, statusEffect));
            }
        }
    }

    CombatChain::CombatChain(CombatChain &&arg) : nodes(std::move(arg.nodes))
    {}

    CombatChain& CombatChain::operator=(CombatChain &&arg)
    {
        nodes = std::move(arg.nodes);
        return *this;
    }

    bool CombatChain::operator==(const CombatChain &arg) const
    {
        return nodes == arg.nodes;
    }

    bool CombatChain::operator!=(const CombatChain &arg) const
    {
        return !(*this == arg);
    }

    CombatChain::Node& CombatChain::AddNode()
    {
        nodes.push_back(Node());
        return nodes.back();
    }

    CombatChain::Node& CombatChain::AddNode(Node &&node)
    {
        nodes.push_back(std::move(node));
        return nodes.back();
    }
}