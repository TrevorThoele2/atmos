#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "MaterialViewCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class StaticMaterialView final :
        public Arca::ClosedTypedRelicAutomation<StaticMaterialView, MaterialViewCore, const Bounds>
    {
    public:
        using Index = int;

        void MaterialIndex(Index to);
        [[nodiscard]] Index MaterialIndex() const;
        void Color(Color to);
        [[nodiscard]] Render::Color Color() const;
        void PatchShader(Arca::Ptr<Asset::ShaderAsset> to);
        [[nodiscard]] Arca::Ptr<Asset::ShaderAsset> PatchShader() const;

        [[nodiscard]] Arca::Ptr<Asset::MaterialAsset> Material() const;
        [[nodiscard]] AxisAlignedBox2D MaterialSlice() const;

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size3D Size() const;
        [[nodiscard]] AxisAlignedBox3D Box() const;

        [[nodiscard]] Arca::Ptr<MaterialViewCore> Core() const;
        [[nodiscard]] Arca::Ptr<const Bounds> Bounds() const;
    public:
        void PostConstruct(ShardTuple shards);
        void Initialize(const Position3D& position);
    private:
        Arca::Ptr<MaterialViewCore> core;
        Arca::Ptr<const Atmos::Bounds> bounds;
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
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::StaticMaterialView, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::StaticMaterialView, BinaryArchive>
    {};
}