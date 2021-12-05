#pragma once

#include "JavaScriptFixture.h"

#include <Atmos/JavaScriptBounds.h>
#include <Atmos/JavaScriptImageCore.h>
#include <Atmos/JavaScriptRenderCore.h>
#include <Atmos/JavaScriptViewSlice.h>

#include <Arca/Create.h>
#include <Atmos/StringUtility.h>

#include <Inscription/Json.h>

template<class>
class JavaScriptShardTestsFixture : public JavaScriptFixture
{
public:
    struct Expectation
    {
        String name;
        String value;
    };

    template<class CreateCommand>
    using ScenarioT = std::tuple<CreateCommand, String, String, String, std::function<void(const String&, Arca::Reliquary&)>>;
public:
    template<class T, std::enable_if_t<std::is_same_v<Spatial::Bounds, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateBounds> Scenario(Arca::RelicID id, Arca::Reliquary& reliquary)
    {
        constexpr auto space = Spatial::Space::World;
        const auto position = Spatial::Point3D
        {
            dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-100, 100))
        };
        const auto baseSize = Spatial::Size2D
        {
            dataGeneration.Random<Spatial::Size2D::Value>(TestFramework::Range<Spatial::Size2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Size2D::Value>(TestFramework::Range<Spatial::Size2D::Value>(-100, 100))
        };
        const auto scalers = Spatial::Scalers2D
        {
            dataGeneration.Random<Spatial::Scalers2D::Value>(TestFramework::Range<Spatial::Scalers2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Scalers2D::Value>(TestFramework::Range<Spatial::Scalers2D::Value>(-100, 100))
        };
        const auto rotation = dataGeneration.Random<Spatial::Angle2D>();

        const auto createCommand = Atmos::Scripting::JavaScript::CreateBounds
        {
            id,
            space,
            position,
            baseSize,
            scalers,
            rotation
        };

        const auto createTraits = "Atmos.Traits.Spatial.CreateBounds";
        const auto traits = "Atmos.Traits.Spatial.Bounds";
        const auto destroyTraits = "Atmos.Traits.Spatial.DestroyBounds";

        const std::function<void(String, Arca::Reliquary&)> expectations = [id, space, position, baseSize, scalers, rotation](
            const String& json, Arca::Reliquary& fieldReliquary)
        {
            Scripting::JavaScript::Bounds bounds;
            Inscription::Json::FromString(bounds, json);

            REQUIRE(bounds.id == id);
            REQUIRE(bounds.space == space);
            REQUIRE(bounds.position.x == Approx(position.x));
            REQUIRE(bounds.position.y == Approx(position.y));
            REQUIRE(bounds.position.z == Approx(position.z));
            REQUIRE(bounds.baseSize.width == Approx(baseSize.width));
            REQUIRE(bounds.baseSize.height == Approx(baseSize.height));
            REQUIRE(bounds.scalers.x == Approx(scalers.x));
            REQUIRE(bounds.scalers.y == Approx(scalers.y));
            REQUIRE(bounds.rotation == Approx(rotation));
            
            const auto index = fieldReliquary.Find<Spatial::Bounds>(id);
            REQUIRE(!index);
        };
        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Render::ImageCore, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateImageCore> Scenario(Arca::RelicID id, Arca::Reliquary& reliquary)
    {
        const auto createCommand = Atmos::Scripting::JavaScript::CreateImageCore
        {
            id,
            std::optional<Atmos::Scripting::JavaScript::ImageAsset>{},
            0
        };

        const auto createTraits = "Atmos.Traits.Render.CreateImageCore";
        const auto traits = "Atmos.Traits.Render.ImageCore";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyImageCore";

        const std::function<void(String, Arca::Reliquary&)> expectations = [id](
            const String& json, Arca::Reliquary& fieldReliquary)
        {
            Scripting::JavaScript::ImageCore imageCore;
            Inscription::Json::FromString(imageCore, json);

            REQUIRE(!imageCore.asset);

            const auto index = fieldReliquary.Find<Render::ImageCore>(id);
            REQUIRE(!index);
        };
        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Render::RenderCore, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateRenderCore> Scenario(Arca::RelicID id, Arca::Reliquary& reliquary)
    {
        const auto createCommand = Atmos::Scripting::JavaScript::CreateRenderCore
        {
            id,
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>{},
            0
        };

        const auto createTraits = "Atmos.Traits.Render.CreateRenderCore";
        const auto traits = "Atmos.Traits.Render.RenderCore";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyRenderCore";

        const std::function<void(String, Arca::Reliquary&)> expectations = [id](
            const String& json, Arca::Reliquary& fieldReliquary)
        {
            Scripting::JavaScript::RenderCore renderCore;
            Inscription::Json::FromString(renderCore, json);

            REQUIRE(!renderCore.material);

            const auto index = fieldReliquary.Find<Render::RenderCore>(id);
            REQUIRE(!index);
        };
        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Render::ViewSlice, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateViewSlice> Scenario(Arca::RelicID id, Arca::Reliquary& reliquary)
    {
        const auto createCommand = Atmos::Scripting::JavaScript::CreateViewSlice
        {
            id,
            Spatial::AxisAlignedBox2D{}
        };

        const auto createTraits = "Atmos.Traits.Render.CreateViewSlice";
        const auto traits = "Atmos.Traits.Render.ViewSlice";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyViewSlice";

        const std::function<void(String, Arca::Reliquary&)> expectations = [id](
            const String& json, Arca::Reliquary& fieldReliquary)
        {
            Scripting::JavaScript::ViewSlice viewSlice;
            Inscription::Json::FromString(viewSlice, json);

            const auto index = fieldReliquary.Find<Render::ViewSlice>(id);
            REQUIRE(!index);
        };
        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }
};