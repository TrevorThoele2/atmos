#pragma once

#include "FileAsset.h"
#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos::Asset
{
    class ImageAssetData;

    class ImageAsset final : public Arca::ClosedTypedRelicAutomation<ImageAsset>, public FileAsset
    {
    public:
        using Dimension = int;
        [[nodiscard]] Dimension Width() const;
        [[nodiscard]] Dimension Height() const;
    public:
        using DataT = ImageAssetData;
        using DataPtr = std::unique_ptr<DataT>;
    public:
        ImageAsset();
        ImageAsset(const ImageAsset& arg) = delete;
        ImageAsset(ImageAsset&& arg) noexcept = default;
        explicit ImageAsset(const ::Inscription::BinaryTableData<ImageAsset>& data);

        [[nodiscard]] DataT* Data();
        [[nodiscard]] const DataT* Data() const;
        template<class RealDataT>
        [[nodiscard]] RealDataT* DataAs();
        template<class RealDataT>
        [[nodiscard]] const RealDataT* DataAs() const;
    public:
        void Initialize(const File::Name& fileName, DataPtr&& data);
    private:
        DataPtr data;
        Dimension width = 0;
        Dimension height = 0;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class RealDataT>
    RealDataT* ImageAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* ImageAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class ImageAssetData
    {
    public:
        virtual ~ImageAssetData() = 0;
        [[nodiscard]] virtual std::unique_ptr<ImageAssetData> Clone() const = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::ImageAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "ImageAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::File::Name& fileName,
            ::Atmos::Asset::ImageAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::ImageAsset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::ImageAsset, BinaryArchive>
    {
        Base<::Atmos::Asset::FileAsset> base;
        ObjectT::Dimension width;
        ObjectT::Dimension height;
    };

    template<>
    class Scribe<::Atmos::Asset::ImageAsset, BinaryArchive> final :
        public ArcaTableScribe<::Atmos::Asset::ImageAsset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}