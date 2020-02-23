#include "StaticMaterialView.h"

#include "ChangeMaterialViewCore.h"

namespace Atmos::Render
{
    void StaticMaterialView::MaterialIndex(Index to) const
    {
        const auto command = CreateModificationCommand(&ChangeMaterialViewCore::index, to);
        Owner().Do<ChangeMaterialViewCore>(command);
    }

    auto StaticMaterialView::MaterialIndex() const -> Index
    {
        return core->materialIndex;
    }

    void StaticMaterialView::Color(Render::Color to) const
    {
        const auto command = CreateModificationCommand(&ChangeMaterialViewCore::color, to);
        Owner().Do<ChangeMaterialViewCore>(command);
    }

    Render::Color StaticMaterialView::Color() const
    {
        return core->color;
    }

    void StaticMaterialView::PatchShader(Arca::Index<Asset::ShaderAsset> to) const
    {
        const auto command = CreateModificationCommand(&ChangeMaterialViewCore::patchShader, to);
        Owner().Do<ChangeMaterialViewCore>(command);
    }

    Arca::Index<Asset::ShaderAsset> StaticMaterialView::PatchShader() const
    {
        return core->patchShader;
    }

    Arca::Index<Asset::MaterialAsset> StaticMaterialView::Material() const
    {
        return core->material;
    }

    AxisAlignedBox2D StaticMaterialView::MaterialSlice() const
    {
        return core->materialSlice;
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

    Arca::Index<MaterialViewCore> StaticMaterialView::Core() const
    {
        return core;
    }

    Arca::Index<const Bounds> StaticMaterialView::Bounds() const
    {
        return bounds;
    }

    StaticMaterialView::StaticMaterialView(Init init)
        : ClosedTypedRelic(init)
    {
        core = FindOrCreate<MaterialViewCore>();
        bounds = FindOrCreate<const Atmos::Bounds>();
    }

    StaticMaterialView::StaticMaterialView(Init init, const Position3D& position, const Size2D& size)
        : ClosedTypedRelic(init)
    {
        core = FindOrCreate<MaterialViewCore>();
        bounds = FindOrCreate<const Atmos::Bounds>(position, size);
    }
}