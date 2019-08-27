#pragma once

#include "FileAsset.h"
#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos::Asset
{
    class ShaderAssetData;

    class ShaderAsset final : public Arca::ClosedTypedRelicAutomation<ShaderAsset>, public FileAsset
    {
    public:
        using DataT = ShaderAssetData;
        using DataPtr = std::unique_ptr<DataT>;
    public:
        ShaderAsset();
        ShaderAsset(const ShaderAsset& arg) = delete;
        ShaderAsset(ShaderAsset&& arg) noexcept;
        explicit ShaderAsset(const ::Inscription::BinaryTableData<ShaderAsset>& data);

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
    RealDataT* ShaderAsset::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* ShaderAsset::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class ShaderAssetData
    {
    public:
        using PassCount = unsigned int;
    public:
        virtual ~ShaderAssetData() = 0;

        [[nodiscard]] virtual std::unique_ptr<ShaderAssetData> Clone() const = 0;
        
        virtual void Reset() = 0;
        virtual void Release() = 0;

        virtual PassCount Begin() const = 0;
        virtual void End() const = 0;

        virtual void BeginNextPass(PassCount pass) const = 0;
        virtual void EndPass() const = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::ShaderAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::File::Name& fileName,
            ::Atmos::Asset::ShaderAsset::DataPtr&& data);
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::ShaderAsset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::ShaderAsset, BinaryArchive>
    {
        Base<::Atmos::Asset::FileAsset> base;
    };

    template<>
    class Scribe<::Atmos::Asset::ShaderAsset, BinaryArchive> final :
        public ArcaTableScribe<::Atmos::Asset::ShaderAsset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}