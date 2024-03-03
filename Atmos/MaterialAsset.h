#pragma once

#include "Asset.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"

namespace Atmos::Asset
{
    enum class MaterialType
    {
        Image,
        Line,
        Region
    };

    class Material final : public Asset<Material>
    {
    public:
        explicit Material(Init init);
        Material(
            Init init,
            const Atmos::Name& name,
            MaterialType type,
            Arca::Index<Shader> vertexShader,
            Arca::Index<Shader> fragmentShader);
        Material(Material&& arg) noexcept;

        Material& operator=(Material&& arg) noexcept;

        [[nodiscard]] MaterialType Type() const;

        [[nodiscard]] Arca::Index<Shader> VertexShader() const;
        [[nodiscard]] Arca::Index<Shader> FragmentShader() const;
    private:
        MaterialType type;

        Arca::Index<Shader> vertexShader;
        Arca::Index<Shader> fragmentShader;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::Material>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::Material";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            Atmos::Asset::MaterialType type,
            Index<Atmos::Asset::Shader> vertexShader,
            Index<Atmos::Asset::Shader> fragmentShader);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::MaterialType, BinaryArchive> final :
        public EnumScribe<Atmos::Asset::MaterialType, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Asset::Material, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::Material, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}