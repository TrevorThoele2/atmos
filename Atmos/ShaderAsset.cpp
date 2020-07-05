#include "ShaderAsset.h"

#include "CreateShaderAssetResource.h"
#include "ShouldCreateAsset.h"

#include "SimpleFile.h"

namespace Atmos::Asset
{
    Shader::Shader(Init init, const Atmos::Name& name, ResourcePtr&& resource, const String& entryPoint) :
        AssetWithResource(init, name, std::move(resource)), entryPoint(entryPoint)
    {}

    Shader::Shader(Init init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization)
    {}

    Shader::Shader(Shader&& arg) noexcept :
        AssetWithResource(std::move(arg)), entryPoint(std::move(arg.entryPoint))
    {}

    Shader& Shader::operator=(Shader&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        entryPoint = arg.entryPoint;
        return *this;
    }

    const String& Shader::EntryPoint() const
    {
        return entryPoint;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Shader>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        const ::Atmos::Asset::Shader::ResourcePtr& data,
        const ::Atmos::String& entryPoint)
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
        archive(object.entryPoint);
        if (archive.IsInput())
        {
            const auto filePath = std::filesystem::current_path() / "Shaders" / object.Name();

            Atmos::SimpleInFile inFile(filePath);
            const auto buffer = inFile.ReadBuffer();
            object.resource = object.Owner().Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{buffer, object.Name() });
        }
    }
}