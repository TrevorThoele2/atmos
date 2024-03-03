#pragma once

namespace Atmos::Asset
{
    class ImageAssetData
    {
    public:
        using Dimension = int;
    public:
        virtual ~ImageAssetData() = 0;

        [[nodiscard]] Dimension Width() const;
        [[nodiscard]] Dimension Height() const;
    protected:
        ImageAssetData(Dimension width, Dimension height);
    private:
        Dimension width;
        Dimension height;
    };
}