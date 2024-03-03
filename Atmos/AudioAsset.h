#pragma once

#include "AssetWithResource.h"
#include "AudioAssetResource.h"

namespace Atmos::Asset
{
    class Audio : public AssetWithResource<Resource::Audio, Audio>
    {
    public:
        explicit Audio(Init init);
        Audio(Init init, const ::Atmos::Name& name, ResourcePtr&& resource);
        Audio(Audio&& arg) noexcept;

        Audio& operator=(Audio&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::Audio>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "AudioAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            const ::Atmos::Asset::Audio::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Audio, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Asset::Audio, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}