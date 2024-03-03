#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "AssetCore.h"

namespace Atmos::Asset
{
    template<class Derived>
    class Asset : public Arca::ClosedTypedRelic<Derived>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<Derived>;
    protected:
        using Init = typename BaseT::Init;
    public:
        explicit Asset(Init init, const Name& name);
        explicit Asset(Init init, Arca::Serialization serialization);
        Asset(const Asset& arg) = delete;
        Asset(Asset&& arg) noexcept = default;

        Asset& operator=(Asset&& arg) noexcept;

        [[nodiscard]] Name Name() const;
        [[nodiscard]] Arca::Index<Core> Core() const;
    private:
        Arca::Index<Atmos::Asset::Core> core;
    };

    template<class Derived>
    Asset<Derived>::Asset(Init init, const Atmos::Name& name) :
        Arca::ClosedTypedRelic<Derived>(init),
        core(init.template Create<Atmos::Asset::Core>(name))
    {}

    template<class Derived>
    Asset<Derived>::Asset(Init init, Arca::Serialization) :
        Arca::ClosedTypedRelic<Derived>(init),
        core(init.template Find<Atmos::Asset::Core>())
    {}

    template<class Derived>
    Asset<Derived>& Asset<Derived>::operator=(Asset&& arg) noexcept
    {
        Arca::ClosedTypedRelic<Derived>::operator=(std::move(arg));
        core = std::move(arg.core);
        return *this;
    }

    template<class Derived>
    Atmos::Name Asset<Derived>::Name() const
    {
        return core->name;
    }

    template<class Derived>
    Arca::Index<Core> Asset<Derived>::Core() const
    {
        return core;
    }
}

namespace Inscription
{
    template<class Derived>
    class Scribe<Atmos::Asset::Asset<Derived>> final
    {
    public:
        using ObjectT = Atmos::Asset::Asset<Derived>;
    public:
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Derived, class Archive>
    struct ScribeTraits<Atmos::Asset::Asset<Derived>, Archive>
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Asset<Derived>>;
    };
}