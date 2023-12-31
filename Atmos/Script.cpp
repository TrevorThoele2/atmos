#include "Script.h"

#include "DataCore.h"

namespace Atmos::Scripting
{
    Script::Script(
        Arca::RelicInit init,
        Arca::Index<Asset::Script> asset,
        const String& executeName,
        Parameters parameters)
        :
        asset(asset),
        executeName(executeName),
        parameters(parameters),
        init(init)
    {
        init.Create<DataCore>();
    }

    Script::Script(Arca::RelicInit init, Arca::Serialization) : init(init)
    {}

    bool Script::operator==(const Script& arg) const
    {
        return IndexEqualsValue(asset, arg.asset)
            && executeName == arg.executeName
            && parameters == arg.parameters
            && isSuspended == arg.isSuspended;
    }

    bool Script::operator!=(const Script& arg) const
    {
        return !(*this == arg);
    }

    Resource* Script::Resource() const
    {
        return resource.get();
    }

    void Script::Setup(ResourcePtr&& set)
    {
        resource = std::move(set);
    }
}