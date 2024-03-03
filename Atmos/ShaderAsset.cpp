#include "ShaderAsset.h"

#include "CreateShaderAssetResource.h"
#include "ShouldCreateAsset.h"

#include "SimpleFile.h"

namespace Atmos::Asset
{
    Shader::Shader(Init init, const Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource))
    {}

    Shader::Shader(Init init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization)
    {}

    Shader::Shader(Shader&& arg) noexcept : AssetWithResource(std::move(arg))
    {}

    Shader& Shader::operator=(Shader&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Shader>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        const ::Atmos::Asset::Shader::ResourcePtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Shader>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::Shader, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Shader, Atmos::Asset::Shader>>(
            object, archive);
        if (archive.IsInput())
        {
            const auto filePath = std::filesystem::current_path() / "shaders" / object.Name();

            Atmos::SimpleInFile inFile(filePath);
            const auto buffer = inFile.ReadBuffer();
            object.resource = object.Owner().Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{buffer, object.Name() });
        }
    }
}