#include "MockImageAssetResource.h"

Asset::ImageSize MockImageAssetResource::Size() const
{
    return Asset::ImageSize{1, 1};
}