#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "ImageCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class DynamicImage final :
        public Arca::ClosedTypedRelic<DynamicImage>
    {
    public:
        DynamicImage(Init init);
        DynamicImage(Init init, const Position3D& position, const Size2D& size);
    public:
        using Index = int;

        void Asset(Arca::Index<Asset::ImageAsset> to);
        [[nodiscard]] Arca::Index<Asset::ImageAsset> Asset() const;
        void AssetIndex(Index to) const;
        [[nodiscard]] Index AssetIndex() const;
        [[nodiscard]] AxisAlignedBox2D AssetSlice() const;

        void Color(Color to) const;
        [[nodiscard]] Render::Color Color() const;

        void Material(Arca::Index<Asset::MaterialAsset> to);
        [[nodiscard]] Arca::Index<Asset::MaterialAsset> Material() const;

        void Position(const Position3D& to) const;
        void Size(const Size2D& to) const;

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;

        [[nodiscard]] Arca::Index<ImageCore> Core() const;
        [[nodiscard]] Arca::Index<Bounds> Bounds() const;
    public:
        void PostConstruct();
        void Initialize();
    private:
        Arca::Index<ImageCore> core;
        Arca::Index<Atmos::Bounds> bounds;
    private:
        template<class CommandT, class MemberT>
        CommandT CreateModificationCommand(std::optional<MemberT> CommandT::*member, MemberT value) const;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class CommandT, class MemberT>
    CommandT DynamicImage::CreateModificationCommand(std::optional<MemberT> CommandT::*member, MemberT value) const
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
    struct Traits<::Atmos::Render::DynamicImage>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "DynamicImage";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::DynamicImage, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Render::DynamicImage, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}