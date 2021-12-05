#pragma once

#include "JavaScriptFixture.h"

#include <Atmos/JavaScriptDiagnosticsStatistics.h>
#include <Atmos/JavaScriptCamera.h>

#include <Atmos/StringUtility.h>
#include "AudioBuffer.h"

#include <Inscription/Json.h>

template<class>
class JavaScriptGlobalRelicTestsFixture : public JavaScriptFixture
{
public:
    struct Expectation
    {
        String name;
        String value;
    };

    template<class T>
    using ScenarioT = std::tuple<String, std::function<void(const String&)>>;
public:
    template<class T, std::enable_if_t<std::is_same_v<Diagnostics::Statistics, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
    {
        const auto statistics = reliquary.Find<Diagnostics::Statistics>();

        const std::function<void(String)> expectations = [id = statistics.ID()](const String& json)
        {
            Scripting::JavaScript::DiagnosticsStatistics statistics;
            Inscription::Json::FromString(statistics, json);

            REQUIRE(statistics.id == id);
        };

        return { "Atmos.Traits.Diagnostics.Statistics", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Render::Camera, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
    {
        const auto camera = reliquary.Find<Render::Camera>();

        const std::function<void(String)> expectations = [id = camera.ID()](const String& json)
        {
            Scripting::JavaScript::Camera camera;
            Inscription::Json::FromString(camera, json);

            REQUIRE(camera.id == id);
        };

        return { "Atmos.Traits.Render.Camera", expectations };
    }
};