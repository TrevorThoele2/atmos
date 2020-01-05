#pragma once

#include <Arca/ReliquaryOrigin.h>

namespace Atmos
{
    void RegisterGlobalTypes(Arca::ReliquaryOrigin& origin);
    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);

    template<class Provider>
    void RegisterProviderComputation(Arca::ReliquaryOrigin& origin)
    {
        using Value = typename Provider::Value;

        origin.Type<Provider>();
        origin.Compute<Value*>(
            [](Arca::Reliquary& reliquary) -> Value*
            {
                const Arca::GlobalPtr<Provider> backing(reliquary);
                return backing->Get();
            });
    }

    template<class Provider>
    void RegisterRedirectionComputation(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
    {
        using Value = typename Provider::Value;

        origin.Compute<Value*>(
            [&globalReliquary](Arca::Reliquary& reliquary) -> Value*
            {
                const Arca::GlobalPtr<Provider> backing(globalReliquary);
                return backing->Get();
            });
    }
}