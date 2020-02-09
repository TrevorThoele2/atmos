#pragma once

#include "Asset.h"

namespace Atmos::Asset
{
    template<class AssetData, class Derived>
    class FileAsset : public Asset<Derived>
    {
    private:
        using BaseT = Asset<Derived>;
    public:
        using FileDataT = AssetData;
        using DataPtr = std::unique_ptr<FileDataT>;
    protected:
        using Init = typename BaseT::Init;
    public:
        explicit FileAsset(Init init);
        FileAsset(Init init, const Atmos::Name& name, DataPtr&& data);
        FileAsset(const FileAsset& arg) = delete;
        FileAsset(FileAsset&& arg) noexcept = default;

        FileAsset& operator=(FileAsset&& arg) noexcept;

        [[nodiscard]] FileDataT* FileData();
        [[nodiscard]] const FileDataT* FileData() const;
        template<class RealFileDataT>
        [[nodiscard]] RealFileDataT* FileDataAs();
        template<class RealFileDataT>
        [[nodiscard]] const RealFileDataT* FileDataAs() const;
    private:
        DataPtr data;
    };

    template<class AssetData, class Derived>
    FileAsset<AssetData, Derived>::FileAsset(Init init) :
        Asset(init)
    {}

    template<class AssetData, class Derived>
    FileAsset<AssetData, Derived>::FileAsset(Init init, const Atmos::Name& name, DataPtr&& data) :
        Asset(init, name),
        data(std::move(data))
    {}

    template<class AssetData, class Derived>
    FileAsset<AssetData, Derived>& FileAsset<AssetData, Derived>::operator=(FileAsset&& arg) noexcept
    {
        Asset<Derived>::operator=(std::move(arg));
        data = std::move(arg.data);
        return *this;
    }

    template<class AssetData, class Derived>
    auto FileAsset<AssetData, Derived>::FileData() -> FileDataT*
    {
        return data.get();
    }

    template<class AssetData, class Derived>
    auto FileAsset<AssetData, Derived>::FileData() const -> const FileDataT*
    {
        return data.get();
    }

    template<class AssetData, class Derived>
    template<class RealFileDataT>
    RealFileDataT* FileAsset<AssetData, Derived>::FileDataAs()
    {
        return static_cast<RealFileDataT*>(data.get());
    }

    template<class AssetData, class Derived>
    template<class RealFileDataT>
    const RealFileDataT* FileAsset<AssetData, Derived>::FileDataAs() const
    {
        return static_cast<RealFileDataT*>(data.get());
    }
}

namespace Inscription
{
    template<class AssetData, class Derived>
    class Scribe<::Atmos::Asset::FileAsset<AssetData, Derived>, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::FileAsset<AssetData, Derived>, BinaryArchive>
    {
    private:
        using BaseT = ArcaCompositeScribe<::Atmos::Asset::FileAsset<AssetData, Derived>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {}
    };
}