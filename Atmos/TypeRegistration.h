#pragma once

#include <Arca/ReliquaryOrigin.h>

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterGlobalTypes(
        Arca::ReliquaryOrigin& origin,
        std::unique_ptr<Input::Manager>&& input,
        std::unique_ptr<Render::GraphicsManager>&& graphics,
        std::unique_ptr<Audio::AudioManager>&& audio);
    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);

    template<class Provider>
    void RegisterProviderComputation(Arca::ReliquaryOrigin& origin, std::unique_ptr<typename Provider::Value>&& initialValue)
    {
        using Value = typename Provider::Value;

        origin.Register<Provider>(std::move(initialValue));
        origin.Compute<Value*>(
            [](Arca::Reliquary& reliquary) -> Value*
            {
                const Arca::GlobalIndex<Provider> backing(reliquary);
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
                const Arca::GlobalIndex<Provider> backing(globalReliquary);
                return backing->Get();
            });
    }
}
