#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "ImageCore.h"
#include "Bounds.h"

namespace Atmos::Render
{
    class StaticImage final :
        public Arca::ClosedTypedRelic<StaticImage>
    {
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

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;

        [[nodiscard]] Arca::Index<ImageCore> Core() const;
        [[nodiscard]] Arca::Index<const Bounds> Bounds() const;
    public:
        explicit StaticImage(Init init);
        StaticImage(Init init, const Position3D& position, const Size2D& size);
    private:
        Arca::Index<ImageCore> core;
        Arca::Index<const Atmos::Bounds> bounds;
    private:
        template<class CommandT, class MemberT>
        CommandT CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class CommandT, class MemberT>
    CommandT StaticImage::CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const
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
    struct Traits<Atmos::Render::StaticImage>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "StaticImage";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::StaticImage, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Render::StaticImage, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override {}
    };
}