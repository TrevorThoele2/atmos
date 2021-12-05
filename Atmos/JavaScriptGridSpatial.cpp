#include "JavaScriptGridSpatial.h"

#include "SpatialAlgorithms.h"

#include "JavaScriptBool.h"
#include "JavaScriptVector.h"
#include "JavaScriptSpace.h"
#include "JavaScriptAxisAlignedBox2D.h"
#include "JavaScriptAxisAlignedBox3D.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptBounds.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptGridPoint.h"
#include "JavaScriptGridAxisAlignedBox.h"
#include "JavaScriptToWorldPoint2D.h"
#include "JavaScriptToWorldPoint3D.h"

#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptTraits.h"
#include <Arca/Destroy.h>

#include "JavaScriptMessage.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<GridSpatialNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "distance", CreateFunction(*isolate, data.context, &OnDistance) },
                { "contains", CreateFunction(*isolate, data.context, &OnContains) },
                { "intersects", CreateFunction(*isolate, data.context, &OnIntersects) },
                { "toPoint", CreateFunction(*isolate, data.context, &OnToPoint) },
                { "toPoint2D", CreateFunction(*isolate, data.context, &OnToPoint2D) },
                { "toPoint3D", CreateFunction(*isolate, data.context, &OnToPoint3D) }
            });
    }

    void Type<GridSpatialNamespace>::OnDistance(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "distance" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Grid::Point, Atmos::Spatial::Grid::Point))V0G0N")));
        };

        const auto point1 = FromV8<Spatial::Grid::Point>(*isolate, info[0]);
        if (point1)
        {
            const auto point2 = FromV8<Spatial::Grid::Point>(*isolate, info[1]);
            if (point2)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Grid::Distance(*point1, *point2)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<GridSpatialNamespace>::OnContains(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "contains" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Grid::AxisAlignedBox, Atmos::Spatial::Grid::Point), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Grid::AxisAlignedBox, Atmos::Spatial::Grid::AxisAlignedBox))V0G0N")));
        };

        const auto box1 = FromV8<Spatial::Grid::AxisAlignedBox>(*isolate, info[0]);
        if (box1)
        {
            const auto point = FromV8<Spatial::Grid::Point>(*isolate, info[1]);
            const auto otherBox = FromV8<Spatial::Grid::AxisAlignedBox>(*isolate, info[1]);
            if (point)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Grid::Contains(*box1, *point)));
            else if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Grid::Contains(*box1, *otherBox)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<GridSpatialNamespace>::OnIntersects(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "intersects" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Grid::AxisAlignedBox, Atmos::Spatial::Grid::AxisAlignedBox))V0G0N")));
        };

        const auto box1 = FromV8<Spatial::Grid::AxisAlignedBox>(*isolate, info[0]);
        if (box1)
        {
            const auto box2 = FromV8<Spatial::Grid::AxisAlignedBox>(*isolate, info[1]);
            if (box2)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Grid::Intersects(*box1, *box2)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<GridSpatialNamespace>::OnToPoint(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toPoint" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Point3D))V0G0N")));
        };

        const auto point3D = FromV8<Spatial::Point2D>(*isolate, info[0]);
        const auto point2D = FromV8<Spatial::Point2D>(*isolate, info[0]);
        if (point3D)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::Grid::ToPoint(*point3D)));
        else if (point2D)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::Grid::ToPoint(*point2D)));
        else
            throwException();
    }

    void Type<GridSpatialNamespace>::OnToPoint2D(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toPoint2D" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Grid::Point))V0G0N")));
        };

        const auto point = FromV8<Spatial::Grid::Point>(*isolate, info[0]);
        if (point)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToPoint2D(*point)));
        else
            throwException();
    }

    void Type<GridSpatialNamespace>::OnToPoint3D(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toPoint3D" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Grid::Point, number))V0G0N")));
        };

        const auto point = FromV8<Spatial::Grid::Point>(*isolate, info[0]);
        const auto z = FromV8<float>(*isolate, info[1]);
        if (point && z)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToPoint3D(*point, *z)));
        else
            throwException();
    }
}