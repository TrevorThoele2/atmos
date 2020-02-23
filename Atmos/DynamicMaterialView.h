#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "MaterialViewCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class DynamicMaterialView final :
        public Arca::ClosedTypedRelic<DynamicMaterialView>
    {
    public:
        DynamicMaterialView(Init init);
        DynamicMaterialView(Init init, const Position3D& position, const Size2D& size);
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

        void Position(const Position3D& to) const;
        void Size(const Size2D& to) const;

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;

        [[nodiscard]] Arca::Index<MaterialViewCore> Core() const;
        [[nodiscard]] Arca::Index<Bounds> Bounds() const;
    public:
        void PostConstruct();
        void Initialize();
    private:
        Arca::Index<MaterialViewCore> core;
        Arca::Index<Atmos::Bounds> bounds;
    private:
        template<class CommandT, class MemberT>
        CommandT CreateModificationCommand(std::optional<MemberT> CommandT::*member, MemberT value) const;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class CommandT, class MemberT>
    CommandT DynamicMaterialView::CreateModificationCommand(std::optional<MemberT> CommandT::*member, MemberT value) const
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
    struct Traits<::Atmos::Render::DynamicMaterialView>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "DynamicMaterialView";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::DynamicMaterialView, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Render::DynamicMaterialView, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}