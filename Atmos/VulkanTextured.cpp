#include "VulkanTextured.h"

#include "VulkanUtilities.h"

#include "RenderAlgorithms.h"
#include "SpatialAlgorithms.h"
#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    Textured::Textured(const Vertices& vertices, const Indices& indices) : vertices(vertices), indices(indices)
    {}

    std::optional<Textured> ToTextured(
        Color color,
        const Spatial::Point2D& worldTopLeft,
        const Spatial::Point2D& textureTopLeft,
        const Spatial::Size2D& baseSize,
        const Spatial::Size2D& textureSize,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation,
        const Spatial::Point2D& rotationCenter,
        const std::vector<Spatial::Point2D>& clipTo)
    {
        const auto clipped = Spatial::Clip(
            Spatial::ToPoints(Spatial::ToAxisAlignedBox2D(0, 0, 1, 1), rotation, rotationCenter), clipTo);
        if (clipped.empty())
            return {};

        std::vector<Spatial::Point2D> baseVertices;
        baseVertices.reserve(clipped.size());
        for (auto& point : clipped)
            baseVertices.emplace_back(
                worldTopLeft.x + point.x * baseSize.width * scalers.x,
                worldTopLeft.y + point.y * baseSize.height * scalers.y);

        std::vector<Spatial::Point2D> texture;
        texture.reserve(clipped.size());
        for (auto& point : clipped)
        {
            const auto rotatedPoint = Spatial::Rotate(point, -rotation, rotationCenter);
            texture.emplace_back(
                (textureTopLeft.x + rotatedPoint.x * baseSize.width) / textureSize.width,
                (textureTopLeft.y + rotatedPoint.y * baseSize.height) / textureSize.height);
        }

        const auto mesh = ConvertToMesh(Triangulate(baseVertices));
        const auto vertices = mesh.vertices;
        const auto indices = mesh.indices;

        if (vertices.size() != texture.size())
            throw GraphicsError("Vertices and texture must have same size.");

        const auto useColor = ToVulkanColor(color);
        std::vector<TexturedVertex> texturedVertices;
        texturedVertices.reserve(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i)
            texturedVertices.push_back(TexturedVertex
                {
                    useColor,
                    glm::vec2{vertices[i].x, vertices[i].y},
                    glm::vec2{texture[i].x, texture[i].y}
                });

        return Textured{ texturedVertices, indices };
    }
}