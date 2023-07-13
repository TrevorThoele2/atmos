#pragma once

#include "JavaScriptFixture.h"

#include <Atmos/JavaScriptActionAsset.h>
#include <Atmos/JavaScriptAudioAsset.h>
#include <Atmos/JavaScriptFontAsset.h>
#include <Atmos/JavaScriptImageAsset.h>
#include <Atmos/JavaScriptMaterialAsset.h>
#include <Atmos/JavaScriptScriptAsset.h>
#include <Atmos/JavaScriptShaderAsset.h>

#include "AudioBuffer.h"
#include <Inscription/Json.h>

template<class>
class JavaScriptAssetTestsFixture : public JavaScriptFixture
{
public:
    using ScenarioT = std::tuple<String, String, std::function<void(const String&)>>;
public:
    template<class T, std::enable_if_t<std::is_same_v<Asset::Action, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();
        constexpr auto boundKey = Input::Key::A;
        Asset::Action::Modifiers boundModifiers = { Input::Key::A };

        const auto index = reliquary.Do(Arca::Create<T>{ name, boundKey, boundModifiers });

        const auto checkBoundModifiers = std::vector<Input::Key>(boundModifiers.begin(), boundModifiers.end());

        const std::function<void(String)> expectations = [id = index.ID(), name, boundKey, checkBoundModifiers](const String& json)
        {
            Scripting::JavaScript::ActionAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
            REQUIRE(asset.boundKey == boundKey);
            REQUIRE(asset.boundModifiers == checkBoundModifiers);
        };

        return { name, "Atmos.Traits.Asset.FindByName.Action", expectations};
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Audio, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Asset::Resource::Audio>{ audioBuffer, name });

        const auto index = reliquary.Do(Arca::Create<T>{ name, std::move(resource) });

        const std::function<void(String)> expectations = [id = index.ID(), name](const String& json)
        {
            Scripting::JavaScript::AudioAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
        };

        return { name, "Atmos.Traits.Asset.FindByName.Audio", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Font, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Asset::Resource::Font>{});

        const auto index = reliquary.Do(Arca::Create<T>{ name, std::move(resource) });

        const std::function<void(String)> expectations = [id = index.ID(), name](const String& json)
        {
            Scripting::JavaScript::FontAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
        };

        return { name, "Atmos.Traits.Asset.FindByName.Font", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Image, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();

        const auto gridSize = Asset::ImageGridSize
        {
            dataGeneration.Random<Asset::ImageGridSize::Dimension>(TestFramework::Range<Asset::ImageGridSize::Dimension>(-100, 100)),
            dataGeneration.Random<Asset::ImageGridSize::Dimension>(TestFramework::Range<Asset::ImageGridSize::Dimension>(-100, 100))
        };

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Asset::Resource::Image>{});

        const auto index = reliquary.Do(Arca::Create<T>{ name, std::move(resource), gridSize });

        const std::function<void(String)> expectations = [id = index.ID(), name, gridSize](const String& json)
        {
            Scripting::JavaScript::ImageAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
            REQUIRE(asset.gridSize.columns == gridSize.columns);
            REQUIRE(asset.gridSize.rows == gridSize.rows);
        };

        return { name, "Atmos.Traits.Asset.FindByName.Image", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Material, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();

        auto scriptResource = reliquary.Do(Atmos::Asset::Resource::Create<Asset::Resource::Script>{});
        const auto scriptAsset = reliquary.Do(Arca::Create<Asset::Script>{ name, std::move(scriptResource) });

        const auto executeName = dataGeneration.Random<String>();

        const auto index = reliquary.Do(Arca::Create<T>{ name, scriptAsset, executeName, Scripting::Parameters{} });

        const std::function<void(String)> expectations = [id = index.ID(), name, scriptAsset, executeName](const String& json)
        {
            Scripting::JavaScript::MaterialAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
            REQUIRE(asset.asset->id == scriptAsset.ID());
            REQUIRE(asset.executeName == executeName);
            REQUIRE(asset.parameters.empty());
        };

        return { name, "Atmos.Traits.Asset.FindByName.Material", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Script, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Asset::Resource::Script>{});

        const auto index = reliquary.Do(Arca::Create<T>{ name, std::move(resource) });

        const std::function<void(String)> expectations = [id = index.ID(), name](const String& json)
        {
            Scripting::JavaScript::ScriptAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
        };

        return { name, "Atmos.Traits.Asset.FindByName.Script", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Shader, T>, int> = 0>
    ScenarioT Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<String>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Asset::Resource::Shader>{});

        const auto index = reliquary.Do(Arca::Create<T>{ name, std::move(resource) });

        const std::function<void(String)> expectations = [id = index.ID(), name](const String& json)
        {
            Scripting::JavaScript::ShaderAsset asset;
            Inscription::Json::FromString(asset, json);

            REQUIRE(asset.id == id);
            REQUIRE(asset.name == name);
        };

        return { name, "Atmos.Traits.Asset.FindByName.Shader", expectations };
    }
};