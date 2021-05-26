#include "VulkanObjectLayering.h"

namespace Atmos::Render::Vulkan
{
    ObjectLayeringKey::ObjectLayeringKey(int space, Spatial::Point3D::Value z) : space(space), z(z)
    {}

    bool ObjectLayeringKey::operator==(const ObjectLayeringKey& arg) const
    {
        return space == arg.space && z == arg.z;
    }

    bool ObjectLayeringKey::operator!=(const ObjectLayeringKey& arg) const
    {
        return !(*this == arg);
    }

    bool ObjectLayeringKey::operator<(const ObjectLayeringKey& arg) const
    {
        if (space != arg.space)
            return space < arg.space;
        else
            return z < arg.z;
    }
}