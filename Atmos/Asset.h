#pragma once

#include "ArcaRelicIncludes.h"
#include "AssetCore.h"

namespace Atmos::Asset
{
    class Asset
    {
    public:
        explicit Asset(Arca::RelicInit init, const Name& name);
        explicit Asset(Arca::RelicInit init, Arca::Serialization serialization);
        Asset(const Asset& arg) = delete;
        Asset(Asset&& arg) noexcept = default;

        Asset& operator=(Asset&& arg) noexcept;

        bool operator==(const Asset& arg) const;
        bool operator!=(const Asset& arg) const;

        [[nodiscard]] Name Name() const;
        [[nodiscard]] Arca::Index<Core> Core() const;
    private:
        Arca::Index<Atmos::Asset::Core> core;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Asset> final
    {
    public:
        using ObjectT = Atmos::Asset::Asset;
    public:
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Asset::Asset, Format>
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Asset>;
    };
}