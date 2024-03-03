#include "StaticMaterialView.h"

namespace Atmos::Render
{
    void StaticMaterialView::MaterialIndex(Index to)
    {
        core->MaterialIndex(to);
    }

    auto StaticMaterialView::MaterialIndex() const -> Index
    {
        return core->MaterialIndex();
    }

    void StaticMaterialView::Color(Render::Color to)
    {
        core->Color(to);
    }

    Render::Color StaticMaterialView::Color() const
    {
        return core->Color();
    }

    void StaticMaterialView::PatchShader(Arca::Ptr<Asset::ShaderAsset> to)
    {
        core->PatchShader(to);
    }

    Arca::Ptr<Asset::ShaderAsset> StaticMaterialView::PatchShader() const
    {
        return core->PatchShader();
    }

    Arca::Ptr<Asset::MaterialAsset> StaticMaterialView::Material() const
    {
        return core->Material();
    }

    AxisAlignedBox2D StaticMaterialView::MaterialSlice() const
    {
        return core->MaterialSlice();
    }

    Position3D StaticMaterialView::Position() const
    {
        return bounds->Position();
    }

    Size3D StaticMaterialView::Size() const
    {
        return bounds->Size();
    }

    AxisAlignedBox3D StaticMaterialView::Box() const
    {
        return bounds->Box();
    }

    Arca::Ptr<MaterialViewCore> StaticMaterialView::Core() const
    {
        return core;
    }

    Arca::Ptr<const Bounds> StaticMaterialView::Bounds() const
    {
        return bounds;
    }

    void StaticMaterialView::PostConstruct(ShardTuple shards)
    {
        core = std::get<0>(shards);
        bounds = std::get<1>(shards);
    }

    void StaticMaterialView::Initialize(const Position3D& position)
    {

    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Render::StaticMaterialView>::typeName = "StaticMaterialView";
}