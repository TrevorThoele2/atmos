#include "VulkanInvalidConduit.h"

namespace Atmos::Render::Vulkan
{
    InvalidConduit::InvalidConduit() : Exception("Attempted to create an invalid conduit.")
    {}
}