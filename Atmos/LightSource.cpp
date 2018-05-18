
#include "LightSource.h"

#include "LightFragment.h"

#include "AxisBoundingBox2D.h"
#include "Canvas.h"

#include "Environment.h"

// TEST
#include "ApproximateLighting.h"
#include "Debug.h"
#include "Math.h"
#include "Position3D.h"
#include "Vector3D.h"
#include "Line3D.h"

namespace Atmos
{
    struct LightPackage
    {
        std::vector<Vector3D> rays;
    };

    struct PixelPosition
    {
        int x, y, z;
        PixelPosition(int x, int y, int z) : x(x), y(y), z(z)
        {}
    };

    LightSource::LightSource(const Color &color, Intensity intensity) : color(color), intensity(intensity)
    {}

    void LightSource::Draw(Canvas &lightmap)
    {
        //if (!IsInBounds())
            //return;

        //DebugScreen::Instance().Misc3().Start();

        //ApproximateLighting approximate;
        //approximate.SolidifySphere(intensity, *GetPosition());
        //int myInt = 1 + 1;

        /*
        typedef GridPosition(*PositionMaker)(GridPosition::ValueT, GridPosition::ValueT, GridPosition::ValueT);
        PositionMaker xy = [](GridPosition::ValueT left, GridPosition::ValueT right, GridPosition::ValueT z)
        {
            return GridPosition(left, right, z);
        };

        PositionMaker yx = [](GridPosition::ValueT left, GridPosition::ValueT right, GridPosition::ValueT z)
        {
            return GridPosition(right, left, z);
        };

        // evenMajor is if the major coordinate is even (has an even amount of positions in it)
        auto make2DCircle = [](PositionMaker positionMaker, GridPosition::ValueT z, float centerMajor, float centerMinor, bool evenMajor, float radius, std::unordered_set<GridPosition> &positions)
        {
            const GridPosition::ValueT gridCenterMajor = GridPosition::DimensionFromPosition(centerMajor);
            const GridPosition::ValueT gridCenterMinor = GridPosition::DimensionFromPosition(centerMinor);

            // How "centered" the center is in the grid position
            const float offsetCenterMajor = std::remainder(centerMajor, GRID_SIZE<float>);
            const float offsetCenterMinor = std::remainder(centerMinor, GRID_SIZE<float>);

            auto emplacePosition = [&](GridPosition::ValueT majorCoord, float minorLength)
            {
                const GridPosition::ValueT minorGrid = static_cast<GridPosition::ValueT>(std::ceil((minorLength + offsetCenterMinor) / GRID_SIZE<float>)) - 1;
                positions.emplace(positionMaker(majorCoord + gridCenterMajor, minorGrid + gridCenterMinor, z));
            };

            auto emplacePositionNegative = [&](GridPosition::ValueT majorCoord, float minorLength)
            {
                const GridPosition::ValueT minorGrid = -1 * static_cast<GridPosition::ValueT>(std::ceil((minorLength - offsetCenterMinor) / GRID_SIZE<float>));
                positions.emplace(positionMaker(majorCoord + gridCenterMajor, minorGrid + gridCenterMinor, z));
            };

            GridPosition::ValueT loop = 0;
            float angle = std::asin(((GRID_SIZE<GridPosition::ValueT> * loop) - offsetCenterMajor) / radius);
            if (!evenMajor)
            {
                // Odd diameter
                // Start off with single column/row of positions
                const float minorLength = std::cos(angle) * radius;
                emplacePosition(loop, minorLength);
                emplacePositionNegative(loop, minorLength);
                ++loop;
                angle = std::asin(((GRID_SIZE<GridPosition::ValueT> * loop) - offsetCenterMajor) / radius);
            }
            else
            {
                // Even diameter
                // Start off with double column/row of positions
                const float minorLength = std::cos(angle) * radius;
                emplacePosition(loop, minorLength);
                emplacePositionNegative(loop, minorLength);

                emplacePosition(-loop + 1, minorLength);
                emplacePositionNegative(-loop + 1, minorLength);

                ++loop;
                angle = std::asin(((GRID_SIZE<GridPosition::ValueT> * loop) - offsetCenterMajor) / radius);
            }

            while (angle <= FULL_CIRCLE_RADIANS<float> / 8.0f)
            {
                const float minorLength = std::cos(angle) * radius;
                emplacePosition(loop, minorLength);
                emplacePositionNegative(loop, minorLength);

                const float otherAngle = std::asin(((GRID_SIZE<GridPosition::ValueT> * (-loop + 1)) - offsetCenterMajor) / radius);
                const float otherMinorLength = std::cos(otherAngle) * radius;
                emplacePosition(-loop, otherMinorLength);
                emplacePositionNegative(-loop, otherMinorLength);

                ++loop;
                angle = std::asin(((GRID_SIZE<GridPosition::ValueT> * loop) - offsetCenterMajor) / radius);
            }
        };

        float centerX = GetPosition()->GetX();
        float centerY = GetPosition()->GetY();
        float centerZ = GetPosition()->GetZ();

        const float offsetCenterZ = std::remainder(centerZ, GRID_SIZE<float>);
        const float intensitySquared = intensity * intensity;

        GridPosition::ValueT zDelta = 0;

        GridPosition::ValueT zLoop = 0;
        GridPosition::ValueT zOffset = 1;
        float radius = intensity;
        float nextRadius = 0.0f;
        auto changeRadius = [&]()
        {
            radius = std::sqrt(intensitySquared - std::pow((zLoop * GRID_SIZE<float>) - offsetCenterZ, 2));
            float zTest = std::pow((zLoop + 1 * GRID_SIZE<float>) - offsetCenterZ, 2);
            nextRadius = (zTest > intensitySquared) ? 0.0f : std::sqrt(intensitySquared - zTest);
            zDelta = GridPosition::ValueT(radius - nextRadius);
        };

        changeRadius();

        // Is Z diameter odd
        if (((GridPosition::DimensionFromPosition(centerZ + intensity) - GridPosition::DimensionFromPosition(centerZ - intensity)) + 1) % 2 == 1)
        {
            const GridPosition::ValueT gridDiameterX = (GridPosition::DimensionFromPosition(centerX + radius) - GridPosition::DimensionFromPosition(centerX - radius)) + 1;
            const GridPosition::ValueT gridDiameterY = (GridPosition::DimensionFromPosition(centerY + radius) - GridPosition::DimensionFromPosition(centerY - radius)) + 1;
            const bool xEven = gridDiameterX % 2 == 0;
            const bool yEven = gridDiameterY % 2 == 0;

            make2DCircle(xy, 0, centerX, centerY, xEven, radius, positions);
            make2DCircle(yx, 0, centerY, centerX, yEven, radius, positions);
            ++zLoop;
            changeRadius();
            zOffset = 0;
        }

        const GridPosition::ValueT gridRadiusZ = GridPosition::DimensionFromPosition(centerZ + intensity) - GridPosition::DimensionFromPosition(centerZ - intensity) / 2;

        while(zLoop <= gridRadiusZ)
        {
            if (radius <= 0.0f)
            {
                ++zLoop;
                changeRadius();
                continue;
            }

            const GridPosition::ValueT gridDiameterX = (GridPosition::DimensionFromPosition(centerX + radius) - GridPosition::DimensionFromPosition(centerX - radius)) + 1;
            const GridPosition::ValueT gridDiameterY = (GridPosition::DimensionFromPosition(centerY + radius) - GridPosition::DimensionFromPosition(centerY - radius)) + 1;
            const bool xEven = gridDiameterX % 2 == 0;
            const bool yEven = gridDiameterY % 2 == 0;

            // Positive circle
            make2DCircle(xy, zLoop, centerX, centerY, xEven, radius, positions);
            make2DCircle(yx, zLoop, centerY, centerX, yEven, radius, positions);

            // Negative circle
            make2DCircle(xy, -zLoop - zOffset, centerX, centerY, xEven, radius, positions);
            make2DCircle(yx, -zLoop - zOffset, centerY, centerX, yEven, radius, positions);

            ++zLoop;
            changeRadius();
        }
        */

        //DebugScreen::Instance().Misc3().Calc();

        /*
        GridPosition::ValueT gridRadius = (intensity / GRID_SIZE);
        GridPosition::ValueT x = gridRadius;
        GridPosition::ValueT y = 0;
        GridPosition::ValueT error = 0;

        while (x >= y)
        {
            positions.emplace(GridPosition(x, y, 0));
            positions.emplace(GridPosition(y, x, 0));
            positions.emplace(GridPosition(-y, x, 0));
            positions.emplace(GridPosition(-x, y, 0));
            positions.emplace(GridPosition(-x, -y, 0));
            positions.emplace(GridPosition(-y, -x, 0));
            positions.emplace(GridPosition(y, -x, 0));
            positions.emplace(GridPosition(x, -y, 0));

            if (error <= 0)
            {
                y += 1;
                error += 2 * y + 1;
            }
            if (error > 0)
            {
                x -= 1;
                error -= 2 * x + 1;
            }
        }
        */

        /*
        const float radiusSquared = intensity * intensity;
        for (GridPosition::ValueT xLoop = GridPosition::DimensionFromPosition(GetPosition()->GetX() - intensity); xLoop < GridPosition::DimensionFromPosition(GetPosition()->GetX() + intensity); ++xLoop)
        {
            const float width = std::sqrt(radiusSquared - (std::pow(xLoop * GRID_SIZE, 2)));
            const int roundedWidth = static_cast<int>(Round(width));
            const GridPosition::ValueT positiveGridWidth = static_cast<GridPosition::ValueT>(std::floor(static_cast<float>(roundedWidth) / static_cast<float>(GRID_SIZE)));
            const GridPosition::ValueT negativeGridWidth = static_cast<GridPosition::ValueT>(std::ceil(static_cast<float>(-roundedWidth) / static_cast<float>(GRID_SIZE)));

            positions.emplace(GridPosition(xLoop, positiveGridWidth, positiveGridWidth));
            positions.emplace(GridPosition(xLoop, negativeGridWidth, positiveGridWidth));
            positions.emplace(GridPosition(xLoop, positiveGridWidth, negativeGridWidth));
            positions.emplace(GridPosition(xLoop, negativeGridWidth, negativeGridWidth));

            int myInt = 1 + 1;
        }
        */

        /*
        DebugScreen::Instance().Misc3().Start();
        const float radiusSquared = intensity * intensity;
        for (float yLoop = -intensity; yLoop != intensity; ++yLoop)
        {
            const float width = std::sqrt(radiusSquared - (yLoop * yLoop));
            const int roundedWidth = static_cast<int>(Round(width));
            directions.push_back(Vector3D(yLoop, roundedWidth, roundedWidth));
            directions.push_back(Vector3D(yLoop, -roundedWidth, roundedWidth));
            directions.push_back(Vector3D(yLoop, roundedWidth, -roundedWidth));
            directions.push_back(Vector3D(yLoop, -roundedWidth, -roundedWidth));
        }
        DebugScreen::Instance().Misc3().Calc();
        */

        //PixelPosition center(512, 384, 0);
        //lightmap.PaintPixel(Canvas::PositionT(center.x, center.y), Color(255, 0, 255, 0));
    }

    void LightSource::SetIntensity(Intensity set)
    {
        intensity = set;
    }

    LightSource::Intensity LightSource::GetIntensity() const
    {
        return intensity;
    }

    void LightSource::SetPosition(const Position3D &set)
    {
        if (!HasPosition())
            return;

        SetPositionImpl(set);
    }

    Optional<Position3D> LightSource::GetPosition() const
    {
        if (!HasPosition())
            return Optional<Position3D>();

        return GetPositionImpl();
    }

    bool LightSource::HasPosition() const
    {
        return HasPositionImpl();
    }
}