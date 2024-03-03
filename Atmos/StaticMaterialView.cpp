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

    void StaticMaterialView::PatchShader(Arca::RelicIndex<Asset::ShaderAsset> to)
    {
        core->PatchShader(to);
    }

    Arca::RelicIndex<Asset::ShaderAsset> StaticMaterialView::PatchShader() const
    {
        return core->PatchShader();
    }

    Arca::RelicIndex<Asset::MaterialAsset> StaticMaterialView::Material() const
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

    Size2D StaticMaterialView::Size() const
    {
        return bounds->Size();
    }

    AxisAlignedBox2D StaticMaterialView::Box() const
    {
        return bounds->Box();
    }

    Arca::ShardIndex<MaterialViewCore> StaticMaterialView::Core() const
    {
        return core;
    }

    Arca::ShardIndex<const Bounds> StaticMaterialView::Bounds() const
    {
        return bounds;
    }

    void StaticMaterialView::PostConstruct()
    {
        core = Create<MaterialViewCore>();
        bounds = Create<const Atmos::Bounds>();
    }

    void StaticMaterialView::Initialize(const Position3D& position, const Size2D& size)
    {
        auto& mutableBounds = const_cast<Atmos::Bounds&>(*bounds);
        mutableBounds.Position(position);
        mutableBounds.Size(size);
    }
}