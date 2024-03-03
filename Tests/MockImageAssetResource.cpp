#include "MockImageAssetResource.h"

MockImageAssetResource::MockImageAssetResource() : size{ 1, 1 }
{}

MockImageAssetResource::MockImageAssetResource(struct Asset::ImageSize size) : size(size)
{}

Asset::ImageSize MockImageAssetResource::Size() const
{
    return size;
}