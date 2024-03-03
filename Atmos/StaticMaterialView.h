#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "MaterialViewCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class StaticMaterialView final :
        public Arca::ClosedTypedRelicAutomation<StaticMaterialView>
    {
    public:
        using Index = int;

        void MaterialIndex(Index to);
        [[nodiscard]] Index MaterialIndex() const;
        void Color(Color to);
        [[nodiscard]] Render::Color Color() const;
        void PatchShader(Arca::RelicIndex<Asset::ShaderAsset> to);
        [[nodiscard]] Arca::RelicIndex<Asset::ShaderAsset> PatchShader() const;

        [[nodiscard]] Arca::RelicIndex<Asset::MaterialAsset> Material() const;
        [[nodiscard]] AxisAlignedBox2D MaterialSlice() const;

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;

        [[nodiscard]] Arca::ShardIndex<MaterialViewCore> Core() const;
        [[nodiscard]] Arca::ShardIndex<const Bounds> Bounds() const;
    public:
        void PostConstruct();
        void Initialize(const Position3D& position, const Size2D& size);
    private:
        Arca::ShardIndex<MaterialViewCore> core;
        Arca::ShardIndex<const Atmos::Bounds> bounds;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::StaticMaterialView>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "StaticMaterialView";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::StaticMaterialView, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Render::StaticMaterialView, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}