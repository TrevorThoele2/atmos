#pragma once

#include "AssetWithResource.h"
#include "AudioAssetResource.h"
#include "LoadAudioAssetResourceData.h"
#include "CreateAudioAssetResource.h"
#include "LoadAssetsUserContext.h"

namespace Atmos::Asset
{
    class Audio : public AssetWithResource<Resource::Audio>
    {
    public:
        Audio(Arca::RelicInit init, const Atmos::Name& name, ResourcePtr&& resource);
        Audio(Arca::RelicInit init, Arca::Serialization serialization);
        Audio(Audio&& arg) noexcept;

        Audio& operator=(Audio&& arg) noexcept;
    public:
        void Setup(ResourcePtr&& set);
    private:
        Arca::RelicInit init;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Audio>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Asset::Audio"; }
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Asset::Audio::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Audio> final
    {
    public:
        using ObjectT = Atmos::Asset::Audio;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Audio>>(
                object, archive);
            if (archive.IsInput())
            {
                auto& assetUserContext = *archive.template UserContext<LoadAssetsUserContext>();

                auto extracted = assetUserContext.LoadAudioData(object.Name());
                if (extracted)
                {
                    using LoadResource = Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Audio>;
                    const auto loaded = object.init.owner.Do(LoadResource
                        {
                            extracted->memory
                        });

                    using CreateResource = Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>;
                    auto resource = object.init.owner.Do(CreateResource
                        {
                            loaded.buffer,
                            object.Name()
                        });

                    object.Setup(std::move(resource));
                }
            }
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Audio, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Audio>;
    };
}