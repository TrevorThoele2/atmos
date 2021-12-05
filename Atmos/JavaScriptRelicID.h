#pragma once

#include <Arca/RelicID.h>
#include "String.h"
#include <Chroma/StringUtility.h>

namespace Inscription
{
    template<class Format>
    void ScrivenRelicID(const std::string& name, Arca::RelicID& id, Format& format)
    {
        if (format.IsOutput())
            format(name, id);
        else
        {
            Atmos::String stringID;
            format(name, stringID);
            id = Chroma::FromString<Arca::RelicID>(stringID);
        }
    }
}