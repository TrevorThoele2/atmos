#pragma once

#include "Asset.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Asset
{
    enum class MaterialType
    {
        Image,
        Line,
        Region
    };

    class Material final : public Asset
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
            Arca::RelicInit init,
            const Atmos::Name& name,
            MaterialType type,
            std::vector<Pass> passes);
        Material(Arca::RelicInit init, Arca::Serialization serialization);
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
    struct Traits<Atmos::Asset::Material>
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
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::MaterialType, Archive> final
    {
        using Category = EnumScribeCategory<Atmos::Asset::MaterialType>;
    };

    template<>
    class Scribe<Atmos::Asset::Material::Pass> final
    {
    public:
        using ObjectT = Atmos::Asset::Material::Pass;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("vertexShader", object.vertexShader);
            archive("fragmentShader", object.fragmentShader);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Material::Pass, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Material::Pass>;
    };

    template<>
    class Scribe<Atmos::Asset::Material> final
    {
    public:
        using ObjectT = Atmos::Asset::Material;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::Asset>(object, archive);
            archive("type", object.type);
            archive("passes", object.passes);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Material, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Material>;
    };
}