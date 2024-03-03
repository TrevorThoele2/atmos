
#pragma once

#include <memory>

#include "Asset.h"

#include "FilePath.h"

namespace Atmos
{
    class ImageAsset : public Asset
    {
    public:
        typedef unsigned short GridDimension;
        typedef unsigned int Dimension;
    public:
        class Data
        {
        private:
            ImageAsset *owner;
            friend ImageAsset;
        public:
            virtual ~Data() = 0 {}
            ImageAsset* GetOwner() const;
        };
    private:
        GridDimension columns, rows;
        Dimension width, height;
        FileName name;

        std::unique_ptr<Data> data;

        ImageAsset(const ImageAsset &arg) = delete;
        ImageAsset& operator=(const ImageAsset &arg) = delete;

        void SetData(Data *set);
        void SetData(std::unique_ptr<Data> &&set);

        String GetStringImpl() const override final;
    public:
        ImageAsset(Data *data, const FileName &fileName, GridDimension columns, GridDimension rows, Dimension width, Dimension height);
        ImageAsset(ImageAsset &&arg);
        ImageAsset& operator=(ImageAsset &&arg);

        bool operator==(const ImageAsset &arg) const;
        bool operator!=(const ImageAsset &arg) const;

        Data* GetData() const;
        template<class DataT>
        DataT* GetData() const;

        void SetColumns(GridDimension set);
        void SetRows(GridDimension set);

        GridDimension GetColumns() const;
        GridDimension GetRows() const;
        Dimension GetWidth() const;
        Dimension GetHeight() const;
        const FileName& GetFileName() const;

        static bool CanMake(const FilePath &path);
        static bool CanMake(void *buffer, std::int32_t size);
    };

    template<class DataT>
    DataT* ImageAsset::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}