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
    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Arca::Reliquary& globalReliquary);

    template<class Provider>
    void RegisterProviderPostulate(Arca::ReliquaryOrigin& origin, std::unique_ptr<typename Provider::Value>&& initialValue)
    {
        using Value = typename Provider::Value;

        origin.Register<Provider>(std::move(initialValue));
        origin.Postulate<Value*>(
            [](Arca::Reliquary& reliquary) -> Value*
            {
                const Arca::Index<Provider> backing(reliquary);
                return backing->Get();
            });
    }

    template<class Provider>
    void RegisterRedirectionPostulate(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary)
    {
        using Value = typename Provider::Value;

        origin.Postulate<Value*>(
            [&globalReliquary](Arca::Reliquary&) -> Value*
            {
                const Arca::Index<Provider> backing(globalReliquary);
                return backing->Get();
            });
    }

    namespace File
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Input
    {
        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);
        Arca::Stage Stage();
    }

    namespace Audio
    {
        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);
    }

    namespace Asset
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Time
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterGlobalRedirectionTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);
    }
}
