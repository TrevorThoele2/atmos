#pragma once

#include <Arca/Curator.h>

#include "ChangeWindowSize.h"
#include "WindowInformation.h"

namespace Atmos::Window
{
    class Curator final : public Arca::Curator
    {
    public:
        Curator(Init init);
    public:
        void Handle(const ChangeSize& command);
    private:
        Arca::Index<Information> information;

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