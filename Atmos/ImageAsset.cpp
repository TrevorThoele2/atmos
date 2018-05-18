
#include "ImageAsset.h"

#include "Environment.h"
#include "DXGraphics.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\Table.h>
#include <Inscription\String.h>

namespace Atmos
{
    ImageAsset* ImageAsset::Data::GetOwner() const
    {
        return owner;
    }

    void ImageAsset::SetData(Data *set)
    {
        data.reset(set);
        data->owner = this;
    }

    void ImageAsset::SetData(std::unique_ptr<Data> &&set)
    {
        data = std::move(set);
        data->owner = this;
    }

    String ImageAsset::GetStringImpl() const
    {
        return name.GetValue();
    }

    ImageAsset::ImageAsset(Data *data, const FileName &name, GridDimension columns, GridDimension rows, Dimension width, Dimension height) : name(name), columns(columns), rows(rows), width(width), height(height)
    {
        SetData(data);
    }

    ImageAsset::ImageAsset(ImageAsset &&arg) : name(std::move(arg.name)), columns(arg.columns), rows(arg.rows), width(arg.width), height(arg.height)
    {
        SetData(std::move(arg.data));
    }

    ImageAsset& ImageAsset::operator=(ImageAsset &&arg)
    {
        SetData(std::move(arg.data));
        name = std::move(arg.name);
        columns = arg.columns;
        rows = arg.rows;
        width = arg.width;
        height = arg.height;
        return *this;
    }

    bool ImageAsset::operator==(const ImageAsset &arg) const
    {
        return Asset::operator==(arg);
    }

    bool ImageAsset::operator!=(const ImageAsset &arg) const
    {
        return !(*this == arg);
    }

    ImageAsset::Data* ImageAsset::GetData() const
    {
        return data.get();
    }

    void ImageAsset::SetColumns(GridDimension set)
    {
        columns = set;
    }

    void ImageAsset::SetRows(GridDimension set)
    {
        rows = set;
    }

    ImageAsset::GridDimension ImageAsset::GetColumns() const
    {
        return columns;
    }

    ImageAsset::GridDimension ImageAsset::GetRows() const
    {
        return rows;
    }

    ImageAsset::Dimension ImageAsset::GetWidth() const
    {
        return width;
    }

    ImageAsset::Dimension ImageAsset::GetHeight() const
    {
        return height;
    }

    const FileName& ImageAsset::GetFileName() const
    {
        return name;
    }

    bool ImageAsset::CanMake(const FilePath &path)
    {
        return Environment::GetGraphics()->CanMakeImage(path);
    }

    bool ImageAsset::CanMake(void *buffer, std::int32_t size)
    {
        return Environment::GetGraphics()->CanMakeImage(buffer, size);
    }
}