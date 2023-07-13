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
        data(asset, executeName, parameters),
        init(init)
    {
        init.Create<DataCore>();
    }

    Script::Script(Arca::RelicInit init, Arca::Serialization) : init(init)
    {}

    bool Script::operator==(const Script& arg) const
    {
        return data == arg.data;
    }

    bool Script::operator!=(const Script& arg) const
    {
        return !(*this == arg);
    }
}