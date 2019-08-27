#include "DynamicMaterialView.h"

namespace Atmos::Render
{
    void MaterialView::MaterialIndex(Index to)
    {
        core->MaterialIndex(to);
    }

    auto MaterialView::MaterialIndex() const -> Index
    {
        return core->MaterialIndex();
    }

    void MaterialView::Color(Render::Color to)
    {
        core->Color(to);
    }

    Render::Color MaterialView::Color() const
    {
        return core->Color();
    }

    void MaterialView::PatchShader(Arca::Ptr<Asset::ShaderAsset> to)
    {
        core->PatchShader(to);
    }

    Arca::Ptr<Asset::ShaderAsset> MaterialView::PatchShader() const
    {
        return core->PatchShader();
    }

    Arca::Ptr<Asset::MaterialAsset> MaterialView::Material() const
    {
        return core->Material();
    }

    AxisAlignedBox2D MaterialView::MaterialSlice() const
    {
        return core->MaterialSlice();
    }

    void MaterialView::Position(const Position3D& to)
    {
        bounds->Position(to);
    }

    void MaterialView::Size(const Size3D& to)
    {
        bounds->Size(to);
    }

    Position3D MaterialView::Position() const
    {
        return bounds->Position();
    }

    Size3D MaterialView::Size() const
    {
        return bounds->Size();
    }

    AxisAlignedBox3D MaterialView::Box() const
    {
        return bounds->Box();
    }

    const Bounds& MaterialView::Bounds() const
    {
        return *bounds;
    }

    void MaterialView::PostConstruct(ShardTuple shards)
    {
        core = std::get<0>(shards);
        bounds = std::get<1>(shards);
    }

    void MaterialView::Initialize(const Position3D& position)
    {

    }
}

namespace Arca
{
    const TypeName Traits<::Atmos::Render::MaterialView>::typeName = "DynamicMaterialView";
}