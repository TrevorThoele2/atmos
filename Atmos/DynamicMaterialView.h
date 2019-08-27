#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "MaterialViewCore.h"
#include "Bounds.h"

#include "Serialization.h"

namespace Atmos::Render
{
    class MaterialView final : public Arca::ClosedTypedRelicAutomation<MaterialView, MaterialViewCore, Bounds>
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

        void Position(const Position3D& to);
        void Size(const Size3D& to);

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size3D Size() const;
        [[nodiscard]] AxisAlignedBox3D Box() const;

        [[nodiscard]] const Bounds& Bounds() const;
    public:
        void PostConstruct(ShardTuple shards);
        void Initialize(const Position3D& position);
    private:
        Arca::Ptr<MaterialViewCore> core;
        Arca::Ptr<Atmos::Bounds> bounds;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::MaterialView>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::MaterialView, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::MaterialView, BinaryArchive>
    {};
}