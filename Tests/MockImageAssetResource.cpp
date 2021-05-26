#include "MockImageAssetResource.h"

MockImageAssetResource::MockImageAssetResource() : size{ 1, 1 }
{}

MockImageAssetResource::MockImageAssetResource(struct Spatial::Size2D size) : size(size)
{}

Spatial::Size2D MockImageAssetResource::Size() const
{
    return size;
}