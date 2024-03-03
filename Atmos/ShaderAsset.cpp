#include "ShaderAsset.h"

#include "CreateShaderAssetData.h"
#include "ShouldCreateAsset.h"

#include "SimpleFile.h"

namespace Atmos::Asset
{
    Shader::Shader(Init init) :
        FileAsset(init)
    {}

    Shader::Shader(Init init, const Atmos::Name& name, DataPtr&& data, const String& entryPoint) :
        FileAsset(init, name, std::move(data)), entryPoint(entryPoint)
    {}

    Shader::Shader(Shader&& arg) noexcept :
        FileAsset(std::move(arg)), entryPoint(std::move(arg.entryPoint))
    {}

    Shader& Shader::operator=(Shader&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
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
        const ::Atmos::Asset::Shader::DataPtr& data,
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
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ShaderData, Atmos::Asset::Shader>>(
            object, archive);
        archive(object.entryPoint);
        if (archive.IsInput())
        {
            const auto filePath = std::filesystem::current_path() / "Shaders" / object.Name();

            Atmos::SimpleInFile inFile(filePath);
            const auto buffer = inFile.ReadBuffer();
            object.data = object.Owner().Do(Atmos::Asset::CreateData<Atmos::Asset::ShaderData>{buffer, object.Name() });
        }
    }
}