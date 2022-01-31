#pragma once

#include "JavaScriptFixture.h"

#include <Atmos/SpatialAlgorithms.h>
#include <Inscription/Json.h>

class JavaScriptSpatialTestsFixture : public JavaScriptFixture
{};

class JavaScriptSpatialDistanceTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, float>;
public:
    static ScenarioT Point2D();
    static ScenarioT Point3D();
private:
    template<class T>
    static ScenarioT CreateScenario(const T& point1, const T& point2)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(point1),
            Inscription::Json::ToString(point2)
        };

        return { parameters, Spatial::Distance(point1, point2) };
    }
};

class JavaScriptSpatialContainsTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, bool>;
public:
    static ScenarioT BoxPoint2D();
    static ScenarioT BoxBox2D();
    static ScenarioT BoxPoint3D();
    static ScenarioT BoxBox3D();
private:
    template<class T, class U>
    static ScenarioT CreateScenario(const T& left, const U& right)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(left),
            Inscription::Json::ToString(right)
        };

        return { parameters, Spatial::Contains(left, right) };
    }
};

class JavaScriptSpatialIntersectsTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, bool>;
public:
    static ScenarioT Box2D();
    static ScenarioT Box3D();
private:
    template<class T>
    static ScenarioT CreateScenario(const T& box1, const T& box2)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(box1),
            Inscription::Json::ToString(box2)
        };

        return { parameters, Spatial::Intersects(box1, box2) };
    }
};

class JavaScriptSpatialEnvelopeTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, std::function<void(const String&)>>;
public:
    static ScenarioT Box2D();
    static ScenarioT Box3D();
private:
    template<class T>
    static ScenarioT CreateScenario(const std::vector<T>& boxes)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(boxes)
        };

        const auto expectation = [boxes](const String& json)
        {
            T returnedBox;
            Inscription::Json::FromString(returnedBox, json);

            const auto expectedBox = Spatial::Envelope(boxes);
            REQUIRE(returnedBox == expectedBox);
        };

        return { parameters, expectation };
    }
};

class JavaScriptSpatialClampTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, std::function<void(const String&)>>;
public:
    static ScenarioT Box2D();
    static ScenarioT Box3D();
private:
    template<class T>
    static ScenarioT CreateScenario(const T& box1, const T& box2)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(box1),
            Inscription::Json::ToString(box2)
        };

        const auto expectation = [box1, box2](const String& json)
        {
            T returnedBox;
            Inscription::Json::FromString(returnedBox, json);

            const auto expectedBox = Spatial::Clamp(box1, box2);
            REQUIRE(returnedBox == expectedBox);
        };

        return { parameters, expectation };
    }
};

class JavaScriptSpatialScaleByTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, std::function<void(const String&)>>;
public:
    static ScenarioT Size2D();
    static ScenarioT Size3D();
private:
    template<class Size, class Scalers>
    static ScenarioT CreateScenario(const Size& size, const Scalers& scalers)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(size),
            Inscription::Json::ToString(scalers)
        };

        const auto expectation = [size, scalers](const String& json)
        {
            Size returnedSize;
            Inscription::Json::FromString(returnedSize, json);

            const auto expectedBox = Spatial::ScaleBy(size, scalers);
            REQUIRE(returnedSize == expectedBox);
        };

        return { parameters, expectation };
    }
};

class JavaScriptSpatialScaleOfTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, std::function<void(const String&)>>;
public:
    static ScenarioT Box2D();
    static ScenarioT Box3D();
private:
    template<class T>
    static ScenarioT CreateScenario(const T& box1, const T& box2)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(box1),
            Inscription::Json::ToString(box2)
        };

        const auto expectation = [box1, box2](const String& json)
        {
            T returnedBox;
            Inscription::Json::FromString(returnedBox, json);

            const auto expectedBox = Spatial::ScaleOf(box1, box2);
            REQUIRE(returnedBox == expectedBox);
        };

        return { parameters, expectation };
    }
};

class JavaScriptSpatialAdditionTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, std::function<void(const String&)>>;
public:
    static ScenarioT Point2D();
    static ScenarioT Point3D();
    static ScenarioT Box2D();
    static ScenarioT Box3D();
private:
    template<class Left, class Right>
    static ScenarioT CreateScenario(const Left& left, const Right& right)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(left),
            Inscription::Json::ToString(right)
        };

        const auto expectation = [left, right](const String& json)
        {
            Left returned;
            Inscription::Json::FromString(returned, json);

            const auto expectedPoint = left + right;
            REQUIRE(returned == expectedPoint);
        };

        return { parameters, expectation };
    }
};

class JavaScriptSpatialSubtractionTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<std::vector<Variant>, std::function<void(const String&)>>;
public:
    static ScenarioT Point2D();
    static ScenarioT Point3D();
private:
    template<class T>
    static ScenarioT CreateScenario(const T& point1, const T& point2)
    {
        std::vector<Variant> parameters =
        {
            Inscription::Json::ToString(point1),
            Inscription::Json::ToString(point2)
        };

        const auto expectation = [point1, point2](const String& json)
        {
            T returnedPoint;
            Inscription::Json::FromString(returnedPoint, json);

            const auto expectedPoint = point1 - point2;
            REQUIRE(returnedPoint == expectedPoint);
        };

        return { parameters, expectation };
    }
};