#include "ShaderAsset.h"

#include "ShouldCreateAsset.h"

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

    void Shader::Setup(ResourcePtr&& set)
    {
        SetResource(std::move(set));
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