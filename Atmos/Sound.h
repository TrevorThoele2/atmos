#pragma once

#include "AudioAsset.h"
#include "SoundCore.h"

namespace Atmos::Audio
{
    class Sound
    {
    public:
        using ResourceT = SoundCore::ResourceT;
        using ResourcePtr = SoundCore::ResourcePtr;
    public:
        virtual ~Sound() = 0;

        Sound& operator=(Sound&& arg) noexcept;

        bool operator==(const Sound& arg) const;
        bool operator!=(const Sound& arg) const;
    public:
        [[nodiscard]] Arca::Index<Asset::Audio> Asset() const;
        [[nodiscard]] Volume Volume() const;
    public:
        [[nodiscard]] ResourceT* Resource();
        [[nodiscard]] const ResourceT* Resource() const;
        template<class RealResourceT>
        [[nodiscard]] RealResourceT* ResourceAs();
        template<class RealResourceT>
        [[nodiscard]] const RealResourceT* ResourceAs() const;

        [[nodiscard]] bool ContainsResource() const;
    protected:
        Sound(Arca::RelicInit init, Arca::Index<Asset::Audio> asset, Audio::Volume volume);
        Sound(Arca::RelicInit init, Arca::Serialization serialization);
        Sound(Sound&& arg) noexcept;
    private:
        Arca::Index<SoundCore> core;
    private:
        Arca::RelicInit init;
    };

    template<class RealResourceT>
    RealResourceT* Sound::ResourceAs()
    {
        return static_cast<RealResourceT*>(core->resource.get());
    }

    template<class RealResourceT>
    const RealResourceT* Sound::ResourceAs() const
    {
        return static_cast<RealResourceT*>(core->resource.get());
    }
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Audio::Sound> final
    {
    public:
        using ObjectT = Atmos::Audio::Sound;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Audio::Sound, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::Sound>;
    };
}