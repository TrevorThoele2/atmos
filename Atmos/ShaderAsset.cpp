#include "ShaderAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Shader::Shader(Arca::RelicInit init, const Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource)),
        init(init)
    {}

    Shader::Shader(Arca::RelicInit init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization),
        init(init)
    {}

    Shader::Shader(Shader&& arg) noexcept :
        AssetWithResource(std::move(arg)),
        init(arg.init)
    {}

    Shader& Shader::operator=(Shader&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }

    bool Shader::operator==(const Shader& arg) const
    {
        return AssetWithResource::operator==(arg);
    }

    bool Shader::operator!=(const Shader& arg) const
    {
        return !(*this == arg);
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
        const Atmos::Name& name,
        const Atmos::Asset::Shader::ResourcePtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Shader>(reliquary, name);
    }
}