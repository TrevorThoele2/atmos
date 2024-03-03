#include "JavaScriptSpatial.h"

#include "SpatialAlgorithms.h"

#include "JavaScriptBool.h"
#include "JavaScriptVector.h"
#include "JavaScriptSpace.h"
#include "JavaScriptAngle3D.h"
#include "JavaScriptAxisAlignedBox2D.h"
#include "JavaScriptAxisAlignedBox3D.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptBounds.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptSize3D.h"
#include "JavaScriptScalers2D.h"
#include "JavaScriptScalers3D.h"
#include "JavaScriptMoveBounds.h"
#include "JavaScriptRotateBounds.h"
#include "JavaScriptScaleBounds.h"
#include "JavaScriptToWorldPoint2D.h"
#include "JavaScriptToWorldPoint3D.h"
#include "JavaScriptGridSpatial.h"

#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptTraits.h"
#include <Arca/Destroy.h>

#include "JavaScriptMessage.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<SpatialNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Space", Type<Spatial::Space>::Register(data) },

                { "left", CreateFunction(*isolate, data.context, &OnLeft) },
                { "top", CreateFunction(*isolate, data.context, &OnTop) },
                { "nearZ", CreateFunction(*isolate, data.context, &OnNearZ) },
                { "right", CreateFunction(*isolate, data.context, &OnRight) },
                { "bottom", CreateFunction(*isolate, data.context, &OnBottom) },
                { "farZ", CreateFunction(*isolate, data.context, &OnFarZ) },

                { "distance", CreateFunction(*isolate, data.context, &OnDistance) },
                { "isBetween", CreateFunction(*isolate, data.context, &OnIsBetween) },
                { "angleOf", CreateFunction(*isolate, data.context, &OnAngleOf) },
                { "contains", CreateFunction(*isolate, data.context, &OnContains) },
                { "intersects", CreateFunction(*isolate, data.context, &OnIntersects) },
                { "envelope", CreateFunction(*isolate, data.context, &OnEnvelope) },
                { "clamp", CreateFunction(*isolate, data.context, &OnClamp) },
                { "scaleBy", CreateFunction(*isolate, data.context, &OnScaleBy) },
                { "scaleOf", CreateFunction(*isolate, data.context, &OnScaleOf) },
                { "cell", CreateFunction(*isolate, data.context, &OnCell) },
                { "add", CreateFunction(*isolate, data.context, &OnAdd) },
                { "subtract", CreateFunction(*isolate, data.context, &OnSubtract) },
                { "toPoint2D", CreateFunction(*isolate, data.context, &OnToPoint2D) },
                { "toPoint3D", CreateFunction(*isolate, data.context, &OnToPoint3D) },
                { "toDegrees", CreateFunction(*isolate, data.context, &OnToDegrees) },
                { "toAxisAlignedBox2D", CreateFunction(*isolate, data.context, &OnToAxisAlignedBox2D) },
                { "toAxisAlignedBox3D", CreateFunction(*isolate, data.context, &OnToAxisAlignedBox3D) },

                { "Grid", Type<GridSpatialNamespace>::Register(data) }
            });
    }

    v8::Local<v8::Value> Type<SpatialNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "MoveBounds", ToV8(*isolate, ArcaTraits::From<Spatial::MoveBounds>()) },
                { "RotateBounds", ToV8(*isolate, ArcaTraits::From<Spatial::RotateBounds>()) },
                { "ScaleBounds", ToV8(*isolate, ArcaTraits::From<Spatial::ScaleBounds>()) },
                { "ToWorldPoint2D", ToV8(*isolate, ArcaTraits::From<Spatial::ToWorldPoint2D>()) },
                { "ToWorldPoint3D", ToV8(*isolate, ArcaTraits::From<Spatial::ToWorldPoint3D>()) },
                { "Bounds", ToV8(*isolate, ArcaTraits::From<Spatial::Bounds>()) },
                { "CreateBounds", ToV8(*isolate, ArcaTraits::From<Arca::Create<Spatial::Bounds>>()) },
                { "DestroyBounds", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Spatial::Bounds>>()) }
            });
    }

	void Type<SpatialNamespace>::OnLeft(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        ExtractCoordinate<Spatial::AxisAlignedBox2D>(info, &Spatial::AxisAlignedBox2D::Left);
    }

    void Type<SpatialNamespace>::OnTop(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        ExtractCoordinate<Spatial::AxisAlignedBox2D>(info, &Spatial::AxisAlignedBox2D::Top);
    }

    void Type<SpatialNamespace>::OnFarZ(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        ExtractCoordinate<Spatial::AxisAlignedBox3D>(info, &Spatial::AxisAlignedBox3D::FarZ);
    }

	void Type<SpatialNamespace>::OnRight(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        ExtractCoordinate<Spatial::AxisAlignedBox2D>(info, &Spatial::AxisAlignedBox2D::Right);
    }

	void Type<SpatialNamespace>::OnBottom(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        ExtractCoordinate<Spatial::AxisAlignedBox2D>(info, &Spatial::AxisAlignedBox2D::Bottom);
    }

    void Type<SpatialNamespace>::OnNearZ(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        ExtractCoordinate<Spatial::AxisAlignedBox3D>(info, &Spatial::AxisAlignedBox3D::NearZ);
    }

    void Type<SpatialNamespace>::OnDistance(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "distance" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D, Atmos::Spatial::Point2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Point3D, Atmos::Spatial::Point3D))V0G0N")));
        };

        const auto from3D = FromV8<Spatial::Point3D>(*isolate, info[0]);
        const auto from2D = FromV8<Spatial::Point2D>(*isolate, info[0]);
        if (from3D)
        {
            const auto to3D = FromV8<Spatial::Point3D>(*isolate, info[1]);
            if (to3D)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Distance(*from3D, *to3D)));
            else
                throwException();
        }
        else if (from2D)
        {
            const auto to2D = FromV8<Spatial::Point2D>(*isolate, info[1]);
            if (to2D)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Distance(*from2D, *to2D)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnIsBetween(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "isBetween" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D, Atmos::Spatial::Point2D, Atmos::Spatial::Point2D))V0G0N")));
        };

        const auto one = FromV8<Spatial::Point2D>(*isolate, info[0]);
        const auto check = FromV8<Spatial::Point2D>(*isolate, info[1]);
        const auto two = FromV8<Spatial::Point2D>(*isolate, info[2]);
        if (one && check && two)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::IsBetween(*one, *check, *two)));
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnAngleOf(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "angleOf" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D, Atmos::Spatial::Point2D, Atmos::Spatial::Point2D))V0G0N")));
        };

        const auto one = FromV8<Spatial::Point2D>(*isolate, info[0]);
        const auto check = FromV8<Spatial::Point2D>(*isolate, info[1]);
        const auto two = FromV8<Spatial::Point2D>(*isolate, info[2]);
        if (one && check && two)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::AngleOf(*one, *check, *two)));
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnContains(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "contains" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox2D, Atmos::Spatial::Point2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox2D, Atmos::Spatial::AxisAlignedBox2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox3D, Atmos::Spatial::Point3D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox3D, Atmos::Spatial::AxisAlignedBox3D))V0G0N")));
        };

        const auto box3D = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[0]);
        const auto box2D = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[0]);
        if (box3D)
        {
            const auto point = FromV8<Spatial::Point3D>(*isolate, info[1]);
            const auto otherBox = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[1]);
            if (point)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Contains(*box3D, *point)));
            else if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Contains(*box3D, *otherBox)));
            else
                throwException();
        }
        else if (box2D)
        {
            const auto point = FromV8<Spatial::Point2D>(*isolate, info[1]);
            const auto otherBox = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[1]);
            if (point)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Contains(*box2D, *point)));
            else if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Contains(*box2D, *otherBox)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnIntersects(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "intersects" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox2D, Atmos::Spatial::AxisAlignedBox2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox3D, Atmos::Spatial::AxisAlignedBox3D))V0G0N")));
        };

        const auto box3D = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[0]);
        const auto box2D = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[0]);
        if (box3D)
        {
            const auto otherBox = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[1]);
            if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Intersects(*box3D, *otherBox)));
            else
                throwException();
        }
        else if (box2D)
        {
            const auto otherBox = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[1]);
            if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Intersects(*box2D, *otherBox)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnEnvelope(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "envelope" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox2D[]), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox3D[]))V0G0N")));
        };

        const auto boxes3D = FromV8<std::vector<Spatial::AxisAlignedBox3D>>(*isolate, info[0]);
        const auto boxes2D = FromV8<std::vector<Spatial::AxisAlignedBox2D>>(*isolate, info[0]);
        if (boxes3D)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::Envelope(*boxes3D)));
        else if (boxes2D)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::Envelope(*boxes2D)));
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnClamp(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "clamp" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox2D, Atmos::Spatial::AxisAlignedBox2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox3D, Atmos::Spatial::AxisAlignedBox3D))V0G0N")));
        };

        const auto box3D = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[0]);
        const auto box2D = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[0]);
        if (box3D)
        {
            const auto otherBox = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[1]);
            if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Clamp(*box3D, *otherBox)));
            else
                throwException();
        }
        else if (box2D)
        {
            const auto otherBox = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[1]);
            if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::Clamp(*box2D, *otherBox)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnScaleBy(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "scaleOf" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Size2D, Atmos::Spatial::Scalers2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Size3D, Atmos::Spatial::Scalers3D))V0G0N")));
        };

        const auto size3D = FromV8<Spatial::Size3D>(*isolate, info[0]);
        const auto size2D = FromV8<Spatial::Size2D>(*isolate, info[0]);
        if (size3D)
        {
            const auto scalers = FromV8<Spatial::Scalers3D>(*isolate, info[1]);
            if (scalers)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ScaleBy(*size3D, *scalers)));
            else
                throwException();
        }
        else if (size2D)
        {
            const auto scalers = FromV8<Spatial::Scalers2D>(*isolate, info[1]);
            if (scalers)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ScaleBy(*size2D, *scalers)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnScaleOf(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "scaleOf" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox2D, Atmos::Spatial::AxisAlignedBox2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::AxisAlignedBox3D, Atmos::Spatial::AxisAlignedBox3D))V0G0N")));
        };

        const auto box3D = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[0]);
        const auto box2D = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[0]);
        if (box3D)
        {
            const auto otherBox = FromV8<Spatial::AxisAlignedBox3D>(*isolate, info[1]);
            if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ScaleOf(*box3D, *otherBox)));
            else
                throwException();
        }
        else if (box2D)
        {
            const auto otherBox = FromV8<Spatial::AxisAlignedBox2D>(*isolate, info[1]);
            if (otherBox)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ScaleOf(*box2D, *otherBox)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnCell(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "cell" has the following signatures: )") +
                String(R"V0G0N((number, number, Atmos::Spatial::Size2D), )V0G0N") +
                String(R"V0G0N((number, number, number, Atmos::Spatial::Size3D))V0G0N")));
        };

        const auto column = FromV8<int>(*isolate, info[0]);
        if (column)
        {
            const auto row = FromV8<int>(*isolate, info[1]);
            if (row)
            {
                const auto depth = FromV8<int>(*isolate, info[2]);
                const auto size2D = FromV8<Spatial::Size2D>(*isolate, info[2]);
                if (depth)
                {
                    const auto size3D = FromV8<Spatial::Size3D>(*isolate, info[3]);
                    if (size3D)
                        info.GetReturnValue().Set(ToV8(*isolate, Spatial::Cell(*column, *row, *depth, *size3D)));
                    else
                        throwException();
                }
                else if (size2D)
                    info.GetReturnValue().Set(ToV8(*isolate, Spatial::Cell(*column, *row, *size2D)));
                else
                    throwException();
            }
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnAdd(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "add" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D, Atmos::Spatial::Point2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Point3D, Atmos::Spatial::Point3D))V0G0N")));
        };

        const auto point3D = FromV8<Spatial::Point3D>(*isolate, info[0]);
        const auto point2D = FromV8<Spatial::Point2D>(*isolate, info[0]);
        if (point3D)
        {
            const auto otherPoint = FromV8<Spatial::Point3D>(*isolate, info[1]);
            if (otherPoint)
                info.GetReturnValue().Set(ToV8(*isolate, *point3D + *otherPoint));
            else
                throwException();
        }
        else if (point2D)
        {
            const auto otherPoint = FromV8<Spatial::Point2D>(*isolate, info[1]);
            if (otherPoint)
                info.GetReturnValue().Set(ToV8(*isolate, *point2D + *otherPoint));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnSubtract(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "subtract" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D, Atmos::Spatial::Point2D), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Point3D, Atmos::Spatial::Point3D))V0G0N")));
        };

        const auto point3D = FromV8<Spatial::Point3D>(*isolate, info[0]);
        const auto point2D = FromV8<Spatial::Point2D>(*isolate, info[0]);
        if (point3D)
        {
            const auto otherPoint = FromV8<Spatial::Point3D>(*isolate, info[1]);
            if (otherPoint)
                info.GetReturnValue().Set(ToV8(*isolate, *point3D - *otherPoint));
            else
                throwException();
        }
        else if (point2D)
        {
            const auto otherPoint = FromV8<Spatial::Point2D>(*isolate, info[1]);
            if (otherPoint)
                info.GetReturnValue().Set(ToV8(*isolate, *point2D - *otherPoint));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnToPoint2D(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toPoint2D" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point3D), )V0G0N") +
                String(R"V0G0N((number, number))V0G0N")));
        };

        const auto point3D = FromV8<Spatial::Point3D>(*isolate, info[0]);
        const auto angle = FromV8<float>(*isolate, info[0]);
        if (point3D)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToPoint2D(*point3D)));
        else if (angle)
        {
            const auto distance = FromV8<float>(*isolate, info[1]);
            if (distance)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToPoint2D(*angle, *distance)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnToPoint3D(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toPoint3D" has the following signatures: )") +
                String(R"V0G0N((Atmos::Spatial::Point2D, number), )V0G0N") +
                String(R"V0G0N((Atmos::Spatial::Angle3D, number))V0G0N")));
        };

        const auto point2D = FromV8<Spatial::Point2D>(*isolate, info[0]);
        const auto angle = FromV8<Spatial::Angle3D>(*isolate, info[0]);
        if (point2D)
        {
            const auto z = FromV8<float>(*isolate, info[1]);
            if (z)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToPoint3D(*point2D, *z)));
            else
                throwException();
        }
        else if (angle)
        {
            const auto distance = FromV8<float>(*isolate, info[1]);
            if (distance)
                info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToPoint3D(*angle, *distance)));
            else
                throwException();
        }
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnToDegrees(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toDegrees" has the following signatures: )") +
                String(R"V0G0N((number))V0G0N")));
        };

        const auto angle = FromV8<Spatial::Angle2D>(*isolate, info[0]);
        if (angle)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToDegrees(*angle)));
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnToAxisAlignedBox2D(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toAxisAlignedBox2D" has the following signatures: )") +
                String(R"V0G0N((number, number, number, number))V0G0N")));
        };

        const auto left = FromV8<float>(*isolate, info[0]);
        const auto top = FromV8<float>(*isolate, info[1]);
        const auto right = FromV8<float>(*isolate, info[2]);
        const auto bottom = FromV8<float>(*isolate, info[3]);
        if (left && top && right && bottom)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToAxisAlignedBox2D(*left, *top, *right, *bottom)));
        else
            throwException();
    }

    void Type<SpatialNamespace>::OnToAxisAlignedBox3D(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto throwException = [&]
        {
            isolate->ThrowException(ToV8(
                *isolate,
                String(R"(The function "toAxisAlignedBox3D" has the following signatures: )") +
                String(R"V0G0N((number, number, number, number, number, number))V0G0N")));
        };

        const auto left = FromV8<float>(*isolate, info[0]);
        const auto top = FromV8<float>(*isolate, info[1]);
        const auto farZ = FromV8<float>(*isolate, info[2]);
        const auto right = FromV8<float>(*isolate, info[3]);
        const auto bottom = FromV8<float>(*isolate, info[4]);
        const auto nearZ = FromV8<float>(*isolate, info[5]);
        if (left && top && farZ && right && bottom && nearZ)
            info.GetReturnValue().Set(ToV8(*isolate, Spatial::ToAxisAlignedBox3D(*left, *top, *farZ, *right, *bottom, *nearZ)));
        else
            throwException();
    }
}