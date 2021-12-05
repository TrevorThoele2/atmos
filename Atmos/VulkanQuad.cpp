#include "VulkanQuad.h"

#include "VulkanUtilities.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render::Vulkan
{
    Quad::Quad(const Vertices& vertices) : vertices(vertices)
    {}

    std::array<QuadVertex, 4> ToQuadVertices(
        Color color,
        Spatial::Point2D center,
        Spatial::Size2D size,
        Spatial::Angle2D rotation,
        Spatial::Point2D rotationCenter,
        Spatial::AxisAlignedBox2D texture)
    {
        const auto rotateAndTranslate = [rotation, rotationCenter, center](const Spatial::Point2D& position)
        {
            const auto sinAngle = std::sin(rotation);
            const auto cosAngle = std::cos(rotation);

            const auto x = position.x - rotationCenter.x;
            const auto y = position.y - rotationCenter.y;

            const auto rotatedX = x * cosAngle - y * sinAngle;
            const auto rotatedY = x * sinAngle + y * cosAngle;

            return Spatial::Point2D
            {
                rotatedX + rotationCenter.x + center.x,
                rotatedY + rotationCenter.y + center.y
            };
        };

        const auto halfWidth = size.width / 2;
        const auto halfHeight = size.height / 2;
        const auto topLeft = rotateAndTranslate({ -halfWidth, -halfHeight });
        const auto topRight = rotateAndTranslate({ halfWidth, -halfHeight });
        const auto bottomLeft = rotateAndTranslate({ -halfWidth, halfHeight });
        const auto bottomRight = rotateAndTranslate({ halfWidth, halfHeight });

        const auto useColor = AtmosToVulkanColor(color);

        return std::array
        {
            QuadVertex
            {
                useColor,
                { topLeft.x, topLeft.y },
                { texture.Left(), texture.Top() }
            },
            QuadVertex
            {
                useColor,
                { topRight.x, topRight.y },
                { texture.Right(), texture.Top() }
            },
            QuadVertex
            {
                useColor,
                { bottomLeft.x, bottomLeft.y },
                { texture.Left(), texture.Bottom() }
            },
            QuadVertex
            {
                useColor,
                { bottomRight.x, bottomRight.y },
                { texture.Right(), texture.Bottom() }
            }
        };
    }
}