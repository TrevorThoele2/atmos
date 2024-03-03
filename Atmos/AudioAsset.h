#pragma once

#include "FileAsset.h"
#include "AudioAssetData.h"

namespace Atmos::Asset
{
    class AudioAsset : public FileAsset<AudioAssetData, AudioAsset>
    {
    public:
        explicit AudioAsset(Init init);
        AudioAsset(Init init, const ::Atmos::Name& name, DataPtr&& data);
        AudioAsset(AudioAsset&& arg) noexcept;

        AudioAsset& operator=(AudioAsset&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::AudioAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "AudioAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            ::Atmos::Asset::AudioAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::AudioAsset, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Asset::AudioAsset, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}