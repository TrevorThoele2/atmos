#include "RenderAlgorithms.h"

#include <map>

#include "SpatialAlgorithms.h"
#include "MathUtility.h"

#include "GridCellSize.h"

namespace Atmos::Render
{
    std::vector<Spatial::Point2D> Simplify(const std::vector<Spatial::Point2D>& points)
    {
        size_t index0 = 0;
        auto usePoints = points;
        while(index0 < usePoints.size() && usePoints.size() >= 3)
        {
            const auto index1 = (index0 + 1) % usePoints.size();
            const auto index2 = (index0 + 2) % usePoints.size();

            if (IsBetween(usePoints[index0], usePoints[index1], usePoints[index2]))
                usePoints.erase(usePoints.begin() + static_cast<int>(index1));
            else
                ++index0;
        }

        return usePoints;
    }

    std::vector<Spatial::Point2D> Triangulate(const std::vector<Spatial::Point2D>& points)
    {
        if (points.size() < 3)
            return std::vector<Spatial::Point2D>{};

        auto usePoints = points;
        std::vector<Spatial::Point2D> returnValue;
        returnValue.reserve(points.size());

        size_t index0 = 0;
        const auto fullCircle = 2 * pi<float>;
        constexpr auto epsilon = std::numeric_limits<float>::epsilon();
        while (usePoints.size() > 3)
        {
            const auto index1 = (index0 + 1) % usePoints.size();
            const auto index2 = (index0 + 2) % usePoints.size();

            const auto calculatedAngle = AngleOf(usePoints[index0], usePoints[index1], usePoints[index2]);
            const auto angle = calculatedAngle < 0
                ? abs(calculatedAngle)
                : fullCircle - calculatedAngle;

            if (angle <= pi<float> + epsilon && angle >= pi<float> - epsilon)
                usePoints.erase(usePoints.begin() + static_cast<int>(index1));
            else if (angle > pi<float>)
                ++index0;
            else
            {
                returnValue.emplace_back(usePoints[index0]);
                returnValue.emplace_back(usePoints[index1]);
                returnValue.emplace_back(usePoints[index2]);

                usePoints.erase(usePoints.begin() + static_cast<int>(index1));
            }

            index0 %= usePoints.size();
        }

        if (!IsBetween(usePoints[0], usePoints[1], usePoints[2]))
        {
            returnValue.emplace_back(usePoints[0]);
            returnValue.emplace_back(usePoints[1]);
            returnValue.emplace_back(usePoints[2]);
        }

        returnValue.shrink_to_fit();
        return returnValue;
    }

    std::vector<Spatial::Point2D> Triangulate(const std::vector<Spatial::Grid::Point>& points)
    {
        auto usePoints = points;
        std::ranges::sort(
            usePoints, 
            [](const Spatial::Grid::Point& left, const Spatial::Grid::Point& right)
            {
                return left.y != right.y ? left.y < right.y : left.x < right.x;
            });

        using Region = std::vector<Spatial::Grid::Point::Value>;
        std::map<Spatial::Grid::Point::Value, std::vector<Region>> decomposition;
        for(auto& point : usePoints)
        {
            auto yFound = decomposition.find(point.y);
            if (yFound == decomposition.end())
                decomposition.emplace(point.y, std::vector<Region>{}).first->second.push_back({ point.x });
            else
            {
                auto wasPushed = false;

                for (auto& region : yFound->second)
                {
                    if (point.x == *region.begin() - 1)
                    {
                        region.insert(region.begin(), point.x);
                        wasPushed = true;
                        break;
                    }
                    else if (point.x == * --region.end() + 1)
                    {
                        region.push_back(point.x);
                        wasPushed = true;
                        break;
                    }
                }

                if (!wasPushed)
                    yFound->second.push_back({ point.x });
            }
        }

        std::vector<Spatial::Point2D> returnValue;
        constexpr auto cellSize = Spatial::Grid::CellSize<Spatial::Point2D::Value>;
        for(auto& y : decomposition)
        {
            for (auto& region : y.second)
            {
                const auto minY = static_cast<float>(y.first) * cellSize;
                const auto maxY = static_cast<float>(y.first) * cellSize + cellSize;

                const auto minX = *region.begin() * cellSize;
                const auto maxX = *--region.end() * cellSize + cellSize;

                returnValue.push_back(Spatial::Point2D{ minX, minY });
                returnValue.push_back(Spatial::Point2D{ maxX, minY });
                returnValue.push_back(Spatial::Point2D{ minX, maxY });
                returnValue.push_back(Spatial::Point2D{ minX, maxY });
                returnValue.push_back(Spatial::Point2D{ maxX, minY });
                returnValue.push_back(Spatial::Point2D{ maxX, maxY });
            }
        }

        return returnValue;
    }

    Mesh ConvertToMesh(const std::vector<Spatial::Point2D>& vertices)
    {
        Mesh returnValue;
        returnValue.vertices.reserve(vertices.size());
        returnValue.indices.reserve(vertices.size());

        std::uint32_t nextIndex = 0;
        std::unordered_map<Spatial::Point2D, std::uint32_t> seenVertices;
        for(auto& vertex : vertices)
        {
            auto found = seenVertices.find(vertex);
            if (found == seenVertices.end())
            {
                seenVertices.emplace(vertex, nextIndex);
                returnValue.vertices.push_back(vertex);
                returnValue.indices.push_back(nextIndex);
                ++nextIndex;
            }
            else
                returnValue.indices.push_back(found->second);
        }

        returnValue.vertices.shrink_to_fit();
        returnValue.indices.shrink_to_fit();
        return returnValue;
    }
}