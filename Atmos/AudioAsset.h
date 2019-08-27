#pragma once

#include "FileAsset.h"
#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos::Asset
{
    class AudioAssetData;
    class AssetPackageCurator;

    class AudioAsset final : public Arca::ClosedTypedRelicAutomation<AudioAsset>, public FileAsset
    {
    public:
        using DataT = AudioAssetData;
        using DataPtr = std::unique_ptr<DataT>;
    public:
        AudioAsset();
        AudioAsset(const AudioAsset& arg) = delete;
        AudioAsset(AudioAsset&& arg) noexcept = default;
        explicit AudioAsset(const ::Inscription::BinaryTableData<AudioAsset>& data);

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
    };

    template<class RealDataT>
    RealDataT* AudioAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* AudioAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class AudioAssetInstanceData;

    class AudioAssetData
    {
    public:
        virtual ~AudioAssetData() = 0;
        [[nodiscard]] virtual std::unique_ptr<AudioAssetData> Clone() const = 0;
        [[nodiscard]] virtual std::unique_ptr<AudioAssetInstanceData> CreateInstanceData() const = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::AudioAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::File::Name& fileName,
            ::Atmos::Asset::AudioAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::AudioAsset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::AudioAsset, BinaryArchive>
    {
        Base<::Atmos::Asset::FileAsset> base;
    };

    template<>
    class Scribe<::Atmos::Asset::AudioAsset, BinaryArchive> final :
        public ArcaTableScribe<::Atmos::Asset::AudioAsset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}