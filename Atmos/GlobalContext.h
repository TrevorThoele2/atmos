#pragma once

namespace Inscription { class Scribe; }

namespace Atmos
{
    template<class T, class Mixin>
    class GlobalContextBase
    {
    public:
        static void Serialize(::Inscription::Scribe &scribe);
    };

    template<class T, class Mixin>
    void GlobalContextBase<T, Mixin>::Serialize(::Inscription::Scribe &scribe)
    {
        Mixin::SerializeImpl(scribe);
    }

    // Defines some global information about a type
    template<class T>
    class GlobalContext;
}