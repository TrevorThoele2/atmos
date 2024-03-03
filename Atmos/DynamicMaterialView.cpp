#include "DynamicMaterialView.h"

namespace Atmos::Render
{
    void DynamicMaterialView::MaterialIndex(Index to)
    {
        core->MaterialIndex(to);
    }

    auto DynamicMaterialView::MaterialIndex() const -> Index
    {
        return core->MaterialIndex();
    }

    void DynamicMaterialView::Color(Render::Color to)
    {
        core->Color(to);
    }

    Render::Color DynamicMaterialView::Color() const
    {
        return core->Color();
    }

    void DynamicMaterialView::PatchShader(Arca::RelicIndex<Asset::ShaderAsset> to)
    {
        core->PatchShader(to);
    }

    Arca::RelicIndex<Asset::ShaderAsset> DynamicMaterialView::PatchShader() const
    {
        return core->PatchShader();
    }

    Arca::RelicIndex<Asset::MaterialAsset> DynamicMaterialView::Material() const
    {
        return core->Material();
    }

    AxisAlignedBox2D DynamicMaterialView::MaterialSlice() const
    {
        return core->MaterialSlice();
    }

    void DynamicMaterialView::Position(const Position3D& to)
    {
        bounds->Position(to);
    }

    void DynamicMaterialView::Size(const Size2D& to)
    {
        bounds->Size(to);
    }

    Position3D DynamicMaterialView::Position() const
    {
        return bounds->Position();
    }

    Size2D DynamicMaterialView::Size() const
    {
        return bounds->Size();
    }

    AxisAlignedBox2D DynamicMaterialView::Box() const
    {
        return bounds->Box();
    }

    Arca::ShardIndex<MaterialViewCore> DynamicMaterialView::Core() const
    {
        return core;
    }

    Arca::ShardIndex<Bounds> DynamicMaterialView::Bounds() const
    {
        return bounds;
    }

    void DynamicMaterialView::PostConstruct()
    {
        core = Create<MaterialViewCore>();
        bounds = Create<Atmos::Bounds>();
    }

    void DynamicMaterialView::Initialize(const Position3D& position, const Size2D& size)
    {
        bounds->Position(position);
        bounds->Size(size);
    }
}