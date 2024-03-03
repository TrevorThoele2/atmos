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
        class Pass
        {
        public:
            Pass() = default;
            Pass(Arca::Index<Shader> vertexShader, Arca::Index<Shader> fragmentShader);

            [[nodiscard]] Arca::Index<Shader> VertexShader() const;
            [[nodiscard]] Arca::Index<Shader> FragmentShader() const;
        private:
            Arca::Index<Shader> vertexShader;
            Arca::Index<Shader> fragmentShader;
        private:
            INSCRIPTION_ACCESS;
        };
    public:
        Material(
            Init init,
            const Atmos::Name& name,
            MaterialType type,
            std::vector<Pass> passes);
        Material(Init init, Arca::Serialization serialization);
        Material(Material&& arg) noexcept;

        Material& operator=(Material&& arg) noexcept;

        [[nodiscard]] std::vector<Pass> Passes() const;
        [[nodiscard]] MaterialType Type() const;
    private:
        MaterialType type = MaterialType::Image;

        std::vector<Pass> passes;
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
            const std::vector<Atmos::Asset::Material::Pass>& passes);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::MaterialType, BinaryArchive> final :
        public EnumScribe<Atmos::Asset::MaterialType, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Asset::Material::Pass, BinaryArchive> final :
        public CompositeScribe<::Atmos::Asset::Material::Pass, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::Atmos::Asset::Material, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::Material, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}