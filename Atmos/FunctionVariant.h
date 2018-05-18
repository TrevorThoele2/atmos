#pragma once

#include "Serialization.h"
#include <Function/Variant.h>

namespace inscription
{
    template<class... Args>
    void Serialize(Scribe &scribe, ::function::Variant<Args...> &obj)
    {
        typedef ::function::Variant<Args...> VariantT;

        if (scribe.IsOutput())
        {
            scribe.Save(obj.GetTypeAsID());
            scribe.Save(obj.GetMaster());
        }
        else // INPUT
        {
            VariantT::ID id;
            scribe.Load(id);

            VariantT::MasterType master;
            scribe.Load(master);

            obj.SetMaster(std::move(master), id);
        }
    }
}