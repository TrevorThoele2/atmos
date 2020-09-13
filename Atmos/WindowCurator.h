#pragma once

#include <Arca/Curator.h>

#include "ChangeWindowSize.h"

namespace Atmos::Window
{
    class Curator final : public Arca::Curator
    {
    public:
        using Arca::Curator::Curator;
    public:
        void Handle(const ChangeSize& command);
    private:
        void ChangeWindowSize(const Spatial::ScreenSize& size);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Window::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Window::ChangeSize>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Window::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Window::Curator>;
    };
}