
#include "ApproximateLighting.h"
#include "GridSize.h"

#include <unordered_map>

namespace Atmos
{
    ApproximateLighting::Node::Node(const RelativeGridPosition &relPosition) : relPosition(relPosition)
    {}

    ApproximateLighting::Node* ApproximateLighting::MakeOrFindNode(const RelativeGridPosition &relPosition)
    {
        return nodes.emplace(relPosition, NodePtr(new Node(relPosition))).first->second.get();
    }

    void ApproximateLighting::MakeSection(GridPosition::ValueT xRadius, GridPosition::ValueT yRadius, GridPosition::ValueT zRadius, float maximumLength)
    {
        const float maximumLengthSquared = maximumLength * maximumLength;

        const GridPosition::ValueT xChange = (xRadius < 0) ? -1 : 1;
        const GridPosition::ValueT yChange = (yRadius < 0) ? -1 : 1;
        const GridPosition::ValueT zChange = (zRadius < 0) ? -1 : 1;

        for (GridPosition::ValueT xLoop = 0; xLoop != xRadius; xLoop += xChange)
        {
            GridPosition::ValueT xSquared = xLoop * xLoop;
            for (GridPosition::ValueT yLoop = 0; yLoop != yRadius; yLoop += yChange)
            {
                GridPosition::ValueT ySquared = yLoop * yLoop;
                for (GridPosition::ValueT zLoop = 0; zLoop != zRadius; zLoop += zChange)
                {
                    GridPosition::ValueT zSquared = zLoop * zLoop;
                    if (xSquared + ySquared + zSquared > maximumLengthSquared)
                        continue;

                    // Make node
                    Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, yLoop, zLoop));
                    MakeOrFindNode(RelativeGridPosition(xLoop + xChange, yLoop, zLoop))->children.push_front(madeNode);
                    MakeOrFindNode(RelativeGridPosition(xLoop, yLoop + yChange, zLoop))->children.push_front(madeNode);
                    MakeOrFindNode(RelativeGridPosition(xLoop, yLoop, zLoop + zChange))->children.push_front(madeNode);
                }
            }
        }

        /*
        for (GridPosition::ValueT zLoop = 0; zLoop <= zRadius; ++zLoop)
        {
            // Loop through the positive x
            GridPosition::ValueT xLoop = 1;
            while (xLoop <= xRadius)
            {
                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, node.relPosition.y, zLoop));
                prev->children.push_front(madeNode);
                prev = madeNode;

                ++xLoop;
            }

            // Loop through the negative x
            prev = &node;
            xLoop = -1;
            while (xLoop >= -xRadius)
            {
                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, node.relPosition.y, zLoop));
                prev->children.push_front(madeNode);
                prev = madeNode;

                --xLoop;
            }
        }

        for (GridPosition::ValueT zLoop = -1; zLoop != -zRadius - 1; --zLoop)
        {
            // Loop through the positive x
            GridPosition::ValueT xLoop = 1;
            while (xLoop <= xRadius)
            {
                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, node.relPosition.y, zLoop));
                prev->children.push_front(madeNode);
                prev = madeNode;

                ++xLoop;
            }

            // Loop through the negative x
            prev = &node;
            xLoop = -1;
            while (xLoop >= -xRadius)
            {
                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, node.relPosition.y, zLoop));
                prev->children.push_front(madeNode);
                prev = madeNode;

                --xLoop;
            }
        }
        */
    }

    ApproximateLighting::ApproximateLighting()
    {
        // x^2 + y^2 + z^2 = r^2
        // y^2 = r^2 - x^2 - z^2
        // y = sqrt(r^2 - x^2 - z^2)
        // x = sqrt(r^2 - y^2 - z^2)

        /*
        const Position3D::ValueT centerX = center.GetX();
        const Position3D::ValueT centerY = center.GetY();
        const Position3D::ValueT centerZ = center.GetZ();

        const Position3D::ValueT centerOffsetX = std::remainder(centerX, GRID_SIZE<Position3D::ValueT>);
        const Position3D::ValueT centerOffsetY = std::remainder(centerY, GRID_SIZE<Position3D::ValueT>);
        const Position3D::ValueT centerOffsetZ = std::remainder(centerZ, GRID_SIZE<Position3D::ValueT>);

        const GridPosition::ValueT gridCenterX = GridPosition::DimensionFromPosition(centerX);
        const GridPosition::ValueT gridCenterY = GridPosition::DimensionFromPosition(centerY);
        const GridPosition::ValueT gridCenterZ = GridPosition::DimensionFromPosition(centerZ);

        const GridPosition::ValueT gridDiameterX = static_cast<GridPosition::ValueT>(std::ceil((centerX + radius) / GRID_SIZE<Position3D::ValueT>)) - GridPosition::DimensionFromPosition(center.GetX() - radius);
        const GridPosition::ValueT gridDiameterY = static_cast<GridPosition::ValueT>(std::ceil((centerY + radius) / GRID_SIZE<Position3D::ValueT>)) - GridPosition::DimensionFromPosition(center.GetY() - radius);
        const GridPosition::ValueT gridDiameterZ = static_cast<GridPosition::ValueT>(std::ceil((centerZ + radius) / GRID_SIZE<Position3D::ValueT>)) - GridPosition::DimensionFromPosition(center.GetZ() - radius);

        const GridPosition::ValueT gridRadiusX = gridDiameterX / 2;
        const GridPosition::ValueT gridRadiusY = gridDiameterY / 2;
        const GridPosition::ValueT gridRadiusZ = gridDiameterZ / 2;

        const bool xEven = gridDiameterX % 2 == 0;
        const bool yEven = gridDiameterY % 2 == 0;
        const bool zEven = gridDiameterZ % 2 == 0;

        // Even dimensions need an extra iteration
        const GridPosition::ValueT loopOffsetX = (xEven) ? 1 : 0;
        const GridPosition::ValueT loopOffsetY = (yEven) ? 1 : 0;
        const GridPosition::ValueT loopOffsetZ = (zEven) ? 1 : 0;

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
        auto make2DCircle = [](PositionMaker positionMaker, GridPosition::ValueT z, GridPosition::ValueT gridCenterMajor, GridPosition::ValueT gridCenterMinor, Position3D::ValueT offsetCenterMajor, Position3D::ValueT offsetCenterMinor, bool evenMajor, float radius, PositionContainer &positions)
        {
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

            // Emplace all positions in an arc for this major
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

        const float radiusSquared = radius * radius;

        GridPosition::ValueT zDelta = 0;

        GridPosition::ValueT zLoop = 0;
        GridPosition::ValueT zOffset = 1;
        float nextRadius = 0.0f;
        auto changeRadius = [&]()
        {
            radius = std::sqrt(radiusSquared - std::pow((zLoop * GRID_SIZE<float>) - centerOffsetZ, 2));
            float zTest = std::pow(((zLoop + 1) * GRID_SIZE<float>) - centerOffsetZ, 2);
            nextRadius = (zTest > radiusSquared) ? 0.0f : std::sqrt(radiusSquared - zTest);
            zDelta = GridPosition::ValueT(std::ceil((radius - nextRadius) / GRID_SIZE<float>));
        };

        // Is Z diameter odd
        if (!zEven)
        {
            changeRadius();
            for (GridPosition::ValueT zInnerOffset = 0; zInnerOffset <= zDelta; ++zInnerOffset)
            {
                make2DCircle(xy, 0, gridCenterX, gridCenterY, centerOffsetX, centerOffsetY, xEven, radius, positions);
                make2DCircle(yx, 0, gridCenterY, gridCenterX, centerOffsetY, centerOffsetX, yEven, radius, positions);
                radius = std::sqrt(radiusSquared - std::pow(((zLoop + zInnerOffset) * GRID_SIZE<float>) - centerOffsetZ, 2));
            }

            ++zLoop;
            zOffset = 0;
        }

        while (zLoop <= gridRadiusZ)
        {
            changeRadius();
            if (radius <= 0.0f)
            {
                ++zLoop;
                continue;
            }

            for (GridPosition::ValueT zInnerOffset = 0; zInnerOffset <= zDelta; ++zInnerOffset)
            {
                // Positive circle
                make2DCircle(xy, zLoop, gridCenterX, gridCenterY, centerOffsetX, centerOffsetY, xEven, radius, positions);
                make2DCircle(yx, zLoop, gridCenterY, gridCenterY, centerOffsetY, centerOffsetX, yEven, radius, positions);

                // Negative circle
                make2DCircle(xy, -zLoop - zOffset, gridCenterX, gridCenterY, centerOffsetX, centerOffsetY, xEven, radius, positions);
                make2DCircle(yx, -zLoop - zOffset, gridCenterY, gridCenterY, centerOffsetY, centerOffsetX, yEven, radius, positions);

                radius = std::sqrt(radiusSquared - std::pow(((zLoop + zInnerOffset) * GRID_SIZE<float>) - centerOffsetZ, 2));
            }

            ++zLoop;
        }
        */

        /*
        for (GridPosition::ValueT zLoop = -gridRadiusZ - loopOffsetZ; zLoop != gridRadiusZ; ++zLoop)
        {
        const float zSquared = Square((zLoop * GRID_SIZE<GridPosition::ValueT>) - centerOffsetX);
        for (GridPosition::ValueT yLoop = -gridRadiusY - loopOffsetY; yLoop != gridRadiusY; ++yLoop)
        {
        const float ySquared = Square((yLoop * GRID_SIZE<GridPosition::ValueT>) - centerOffsetY);
        for (GridPosition::ValueT xLoop = -gridRadiusX - loopOffsetX; xLoop != gridRadiusX; ++xLoop)
        {
        const float xSquared = Square((xLoop * GRID_SIZE<GridPosition::ValueT>) - centerOffsetX);
        if (zSquared + ySquared + xSquared <= radiusSquared)
        {
        set.emplace(GridPosition(xLoop, yLoop, zLoop));
        int myInt = 1 + 1;
        }
        }
        }
        }
        */

        /*
        GridPosition::ValueT loopOffset = 0;

        // If z grid diameter is odd
        if (gridDiameterZ % 2 == 1)
        {
        for (GridPosition::ValueT xLoop = 0; xLoop <= gridRadiusX; ++xLoop)
        {
        const float y = std::sqrt(radiusSquared - Square((xLoop * GRID_SIZE<GridPosition::ValueT>) - centerOffsetX));
        auto emplaced = set.emplace(GridPosition(xLoop, GridPosition::DimensionFromPosition(y + centerOffsetY), 0)).first;
        int myInt = 1 + 1;
        }

        loopOffset = 1;
        }

        for (GridPosition::ValueT zLoop = loopOffset; zLoop <= gridRadiusZ; ++zLoop)
        {
        GridPosition::ValueT zPlace = zLoop;
        const GridPosition::ValueT zRealSquared = static_cast<GridPosition::ValueT>(Square((zLoop * GRID_SIZE<GridPosition::ValueT>) - centerOffsetZ));
        auto xLooper = [&]()
        {
        for (GridPosition::ValueT xLoop = loopOffset; xLoop <= gridRadiusX; ++xLoop)
        {
        const float y = std::sqrt(radiusSquared - Square((xLoop * GRID_SIZE<GridPosition::ValueT>) - centerOffsetX) - zRealSquared);
        auto emplaced = set.emplace(GridPosition(xLoop, GridPosition::DimensionFromPosition(y + centerOffsetY), zPlace)).first;
        int myInt = 1 + 1;
        }
        };

        xLooper();
        zPlace *= -1;
        xLooper();
        }
        */
    }

    void ApproximateLighting::SolidifySphere(float radius, const Position3D &center)
    {
        constexpr size_t maxSize = 60000;
        static std::vector<RelativeGridPosition> test;
        static bool once = false;
        if (!once)
        {
            for (RelativeGridPosition::ValueT xLoop = 0; xLoop != maxSize; ++xLoop)
                test.push_back(RelativeGridPosition(xLoop, 0, 0));
            once = true;
        }
        else
        {
            static RelativeGridPosition::ValueT currentVal = 0;
            for (auto &itr : test)
                itr.x = currentVal++;
        }

        //std::list<RelativeGridPosition> test;
        //for (RelativeGridPosition::ValueT xLoop = 0; xLoop != 1000000; ++xLoop)
            //test.push_back(RelativeGridPosition(xLoop, 0, 0));

        /*
        if (radius <= 0.0f)
            return;

        const GridPosition::ValueT gridDiameterX = static_cast<GridPosition::ValueT>(std::ceil((center.GetX() + radius) / GRID_SIZE<Position3D::ValueT>)) - GridPosition::DimensionFromPosition(center.GetX() - radius);
        const GridPosition::ValueT gridDiameterY = static_cast<GridPosition::ValueT>(std::ceil((center.GetY() + radius) / GRID_SIZE<Position3D::ValueT>)) - GridPosition::DimensionFromPosition(center.GetY() - radius);
        const GridPosition::ValueT gridDiameterZ = static_cast<GridPosition::ValueT>(std::ceil((center.GetZ() + radius) / GRID_SIZE<Position3D::ValueT>)) - GridPosition::DimensionFromPosition(center.GetZ() - radius);

        const GridPosition::ValueT gridRadiusX = gridDiameterX / 2;
        const GridPosition::ValueT gridRadiusY = gridDiameterY / 2;
        const GridPosition::ValueT gridRadiusZ = gridDiameterZ / 2;

        Node *prev = nullptr;

        RelativeGridPosition currentPosition(0, 0, 0);
        head = MakeOrFindNode(currentPosition);
        prev = head;

        // Create all positions in the x, y, and z positions
        const float maximumLengthSquared = radius * radius;
        for (GridPosition::ValueT xLoop = 1; xLoop != gridRadiusX + 1; ++xLoop)
        {
            const GridPosition::ValueT xLoopSquared = xLoop * xLoop;
            for (GridPosition::ValueT zLoop = 1; zLoop != gridRadiusZ + 1; ++zLoop)
            {
                const GridPosition::ValueT zLoopSquared = zLoop * zLoop;
                if (xLoopSquared + zLoopSquared > maximumLengthSquared)
                    continue;

                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop));
                MakeOrFindNode(RelativeGridPosition(xLoop - 1, 0, zLoop))->children.push_front(madeNode);
                MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop - 1))->children.push_front(madeNode);
            }

            for (GridPosition::ValueT zLoop = -1; zLoop != -gridRadiusZ - 1; --zLoop)
            {
                const GridPosition::ValueT zLoopSquared = zLoop * zLoop;
                if (xLoopSquared + zLoopSquared > maximumLengthSquared)
                    continue;

                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop));
                MakeOrFindNode(RelativeGridPosition(xLoop - 1, 0, zLoop))->children.push_front(madeNode);
                MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop - 1))->children.push_front(madeNode);
            }
        }

        for (GridPosition::ValueT xLoop = -1; xLoop != -gridRadiusX - 1; --xLoop)
        {
            const GridPosition::ValueT xLoopSquared = xLoop * xLoop;
            for (GridPosition::ValueT zLoop = 1; zLoop != gridRadiusZ + 1; ++zLoop)
            {
                const GridPosition::ValueT zLoopSquared = zLoop * zLoop;
                if (xLoopSquared + zLoopSquared > maximumLengthSquared)
                    continue;

                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop));
                MakeOrFindNode(RelativeGridPosition(xLoop - 1, 0, zLoop))->children.push_front(madeNode);
                MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop - 1))->children.push_front(madeNode);
            }

            for (GridPosition::ValueT zLoop = -1; zLoop != -gridRadiusZ - 1; --zLoop)
            {
                const GridPosition::ValueT zLoopSquared = zLoop * zLoop;
                if (xLoopSquared + zLoopSquared > maximumLengthSquared)
                    continue;

                Node *madeNode = MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop));
                MakeOrFindNode(RelativeGridPosition(xLoop - 1, 0, zLoop))->children.push_front(madeNode);
                MakeOrFindNode(RelativeGridPosition(xLoop, 0, zLoop - 1))->children.push_front(madeNode);
            }
        }

        MakeSection(gridRadiusX, gridRadiusY, gridRadiusZ, radius);
        MakeSection(-gridRadiusX, gridRadiusY, gridRadiusZ, radius);
        MakeSection(gridRadiusX, -gridRadiusY, gridRadiusZ, radius);
        MakeSection(-gridRadiusX, -gridRadiusY, gridRadiusZ, radius);
        MakeSection(gridRadiusX, gridRadiusY, -gridRadiusZ, radius);
        MakeSection(-gridRadiusX, gridRadiusY, -gridRadiusZ, radius);
        MakeSection(gridRadiusX, -gridRadiusY, -gridRadiusZ, radius);
        MakeSection(-gridRadiusX, -gridRadiusY, -gridRadiusZ, radius);
        */
    }
}