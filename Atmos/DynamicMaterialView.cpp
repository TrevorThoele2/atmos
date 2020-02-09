#include "DynamicMaterialView.h"

#include "ChangeMaterialViewCore.h"
#include "MoveBounds.h"
#include "ResizeBounds.h"

namespace Atmos::Render
{
    DynamicMaterialView::DynamicMaterialView(Init init)
        : ClosedTypedRelic(init)
    {
        core = FindOrCreate<MaterialViewCore>();
        bounds = FindOrCreate<Atmos::Bounds>();
    }

    DynamicMaterialView::DynamicMaterialView(
        Init init, const Position3D& position, const Size2D& size)
        :
        ClosedTypedRelic(init)
    {
        core = Create<MaterialViewCore>();
        bounds = Create<Atmos::Bounds>(position, size);
    }

    void DynamicMaterialView::MaterialIndex(Index to) const
    {
        const auto command = CreateModificationCommand(&ChangeMaterialViewCore::index, to);
        Owner().Do<ChangeMaterialViewCore>(command);
    }

    auto DynamicMaterialView::MaterialIndex() const -> Index
    {
        return core->materialIndex;
    }

    void DynamicMaterialView::Color(Render::Color to) const
    {
        const auto command = CreateModificationCommand(&ChangeMaterialViewCore::color, to);
        Owner().Do<ChangeMaterialViewCore>(command);
    }

    Render::Color DynamicMaterialView::Color() const
    {
        return core->color;
    }

    void DynamicMaterialView::PatchShader(Arca::RelicIndex<Asset::ShaderAsset> to) const
    {
        const auto command = CreateModificationCommand(&ChangeMaterialViewCore::patchShader, to);
        Owner().Do<ChangeMaterialViewCore>(command);
    }

    Arca::RelicIndex<Asset::ShaderAsset> DynamicMaterialView::PatchShader() const
    {
        return core->patchShader;
    }

    Arca::RelicIndex<Asset::MaterialAsset> DynamicMaterialView::Material() const
    {
        return core->material;
    }

    AxisAlignedBox2D DynamicMaterialView::MaterialSlice() const
    {
        return core->materialSlice;
    }

    void DynamicMaterialView::Position(const Position3D& to) const
    {
        Owner().Do<MoveBounds>(ID(), to);
    }

    void DynamicMaterialView::Size(const Size2D& to) const
    {
        Owner().Do<ResizeBounds>(ID(), to);
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
}