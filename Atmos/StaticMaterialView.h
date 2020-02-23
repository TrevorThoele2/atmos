#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "MaterialViewCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class StaticMaterialView final :
        public Arca::ClosedTypedRelic<StaticMaterialView>
    {
    public:
        using Index = int;

        void MaterialIndex(Index to) const;
        [[nodiscard]] Index MaterialIndex() const;
        void Color(Color to) const;
        [[nodiscard]] Render::Color Color() const;
        void PatchShader(Arca::Index<Asset::ShaderAsset> to) const;
        [[nodiscard]] Arca::Index<Asset::ShaderAsset> PatchShader() const;

        [[nodiscard]] Arca::Index<Asset::MaterialAsset> Material() const;
        [[nodiscard]] AxisAlignedBox2D MaterialSlice() const;

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;

        [[nodiscard]] Arca::Index<MaterialViewCore> Core() const;
        [[nodiscard]] Arca::Index<const Bounds> Bounds() const;
    public:
        explicit StaticMaterialView(Init init);
        StaticMaterialView(Init init, const Position3D& position, const Size2D& size);
    private:
        Arca::Index<MaterialViewCore> core;
        Arca::Index<const Atmos::Bounds> bounds;
    private:
        template<class CommandT, class MemberT>
        CommandT CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class CommandT, class MemberT>
    CommandT StaticMaterialView::CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const
    {
        CommandT command;
        command.id = ID();
        (command.*member) = value;
        return command;
    }
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