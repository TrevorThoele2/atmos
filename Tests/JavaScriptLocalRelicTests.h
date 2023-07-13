#pragma once

#include "JavaScriptFixture.h"

#include <Atmos/JavaScriptActionAsset.h>
#include <Atmos/JavaScriptAudioAsset.h>
#include <Atmos/JavaScriptFontAsset.h>
#include <Atmos/JavaScriptImageAsset.h>
#include <Atmos/JavaScriptMaterialAsset.h>
#include <Atmos/JavaScriptScriptAsset.h>
#include <Atmos/JavaScriptShaderAsset.h>

#include <Atmos/JavaScriptPositionedSound.h>
#include <Atmos/JavaScriptUniversalSound.h>
#include <Atmos/JavaScriptEntity.h>
#include <Atmos/JavaScriptUIImage.h>
#include <Atmos/JavaScriptUIText.h>
#include <Atmos/JavaScriptDynamicImage.h>
#include <Atmos/JavaScriptDynamicText.h>
#include <Atmos/JavaScriptGridRegion.h>
#include <Atmos/JavaScriptLine.h>
#include <Atmos/JavaScriptScript.h>

#include <Arca/Create.h>
#include <Atmos/StringUtility.h>
#include "AudioBuffer.h"
#include <Atmos/CompileScript.h>

#include <Inscription/Json.h>
#include <Inscription/Plaintext.h>

template<class>
class JavaScriptLocalRelicTestsFixture : public JavaScriptFixture
{
public:
    struct Expectation
    {
        String name;
        String value;
    };

    template<class T>
    using ScenarioT = std::tuple<Arca::Index<T>, String, std::function<void(const String&)>>;
public:
    template<class T, std::enable_if_t<std::is_same_v<Asset::Action, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Action", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Audio, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Audio", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Font, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Font", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Image, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Image", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Material, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Material", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Script, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Script", expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Asset::Shader, T>, int> = 0>
    ScenarioT<T> Scenario(Arca::Reliquary& reliquary)
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

        return { index, "Atmos.Traits.Asset.Shader", expectations };
    }
};

template<class>
class JavaScriptCreatableLocalRelicTestsFixture : public JavaScriptFixture
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
    template<class T, std::enable_if_t<std::is_same_v<Audio::PositionedSound, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreatePositionedSound> Scenario(Arca::Reliquary& reliquary)
    {
        const auto assetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>{
            Atmos::Buffer{}, assetName });
        const auto asset = reliquary.Do(Arca::Create<Atmos::Asset::Audio>{
            assetName, std::move(resource) });

        const auto volume = dataGeneration.Random<Atmos::Audio::Volume>();

        const auto position = dataGeneration.RandomStack<
            Atmos::Spatial::Point3D, Atmos::Spatial::Point3D::Value, Atmos::Spatial::Point3D::Value, Atmos::Spatial::Point3D::Value>();

        const auto createCommand = Atmos::Scripting::JavaScript::CreatePositionedSound
        {
            Atmos::Scripting::JavaScript::AudioAsset{ asset.ID(), assetName },
            volume,
            position
        };

        const auto createTraits = "Atmos.Traits.Audio.CreatePositionedSound";
        const auto traits = "Atmos.Traits.Audio.PositionedSound";
        const auto destroyTraits = "Atmos.Traits.Audio.DestroyPositionedSound";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [asset, volume](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::PositionedSound sound;
            Inscription::Json::FromString(sound, json);

            REQUIRE(sound.asset);
            REQUIRE(sound.asset->id == asset.ID());
            REQUIRE(sound.volume == Approx(volume));

            const auto batch = reliquary.Batch<Audio::PositionedSound>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Audio::UniversalSound, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateUniversalSound> Scenario(Arca::Reliquary& reliquary)
    {
        const auto assetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>{
            Atmos::Buffer{}, assetName });
        const auto asset = reliquary.Do(Arca::Create<Atmos::Asset::Audio>{
            assetName, std::move(resource) });

        const auto volume = dataGeneration.Random<Atmos::Audio::Volume>();

        const auto createCommand = Atmos::Scripting::JavaScript::CreateUniversalSound
        {
            Atmos::Scripting::JavaScript::AudioAsset{ asset.ID(), assetName },
            volume
        };

        const auto createTraits = "Atmos.Traits.Audio.CreateUniversalSound";
        const auto traits = "Atmos.Traits.Audio.UniversalSound";
        const auto destroyTraits = "Atmos.Traits.Audio.DestroyUniversalSound";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [asset, volume](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::UniversalSound sound;
            Inscription::Json::FromString(sound, json);

            REQUIRE(sound.asset);
            REQUIRE(sound.asset->id == asset.ID());
            REQUIRE(sound.volume == Approx(volume));

            const auto batch = reliquary.Batch<Audio::UniversalSound>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Entity::Entity, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateEntity> Scenario(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        const auto createCommand = Atmos::Scripting::JavaScript::CreateEntity
        {
            name,
            position,
            isSolid,
            tags
        };

        const auto createTraits = "Atmos.Traits.Entity.CreateEntity";
        const auto traits = "Atmos.Traits.Entity.Entity";
        const auto destroyTraits = "Atmos.Traits.Entity.DestroyEntity";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [name, position, isSolid, tags](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::Entity entity;
            Inscription::Json::FromString(entity, json);

            REQUIRE(entity.name == name);
            REQUIRE(entity.position.x == position.x);
            REQUIRE(entity.position.y == position.y);
            REQUIRE(entity.isSolid == isSolid);
            REQUIRE(entity.tags == tags);

            const auto batch = reliquary.Batch<Atmos::Entity::Entity>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::UI::Image, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateUIImage> Scenario(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::Buffer{}, imageAssetName, Atmos::Spatial::Size2D{ 1, 1 }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(resource), Atmos::Asset::ImageGridSize{ 1, 1 } });

        const auto createCommand = Atmos::Scripting::JavaScript::CreateUIImage
        {
            Atmos::Scripting::JavaScript::ImageAsset{ imageAsset.ID(), imageAssetName },
            0,
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>(),
            Render::Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            0.0f
        };

        const auto createTraits = "Atmos.Traits.UI.CreateImage";
        const auto traits = "Atmos.Traits.UI.Image";
        const auto destroyTraits = "Atmos.Traits.UI.DestroyImage";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::UIImage image;
            Inscription::Json::FromString(image, json);

            REQUIRE(image.id > 0);

            const auto batch = reliquary.Batch<Atmos::UI::Image>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::UI::Text, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateUIText> Scenario(Arca::Reliquary& reliquary)
    {
        const auto fontAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>{
            Atmos::Buffer{}, fontAssetName });
        const auto fontAsset = reliquary.Do(Arca::Create<Atmos::Asset::Font>{
            fontAssetName, std::move(resource) });

        const auto createCommand = Atmos::Scripting::JavaScript::CreateUIText
        {
            "",
            Atmos::Scripting::JavaScript::FontAsset{ fontAsset.ID(), fontAssetName },
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>(),
            0.0f,
            false,
            false,
            Render::Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            0.0f
        };

        const auto createTraits = "Atmos.Traits.UI.CreateText";
        const auto traits = "Atmos.Traits.UI.Text";
        const auto destroyTraits = "Atmos.Traits.UI.DestroyText";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::UIText text;
            Inscription::Json::FromString(text, json);

            REQUIRE(text.id > 0);

            const auto batch = reliquary.Batch<Atmos::UI::Text>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::DynamicImage, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateDynamicImage> Scenario(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::Buffer{}, imageAssetName, Atmos::Spatial::Size2D{ 1, 1 }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(resource), Atmos::Asset::ImageGridSize{ 1, 1 } });

        const auto createCommand = Atmos::Scripting::JavaScript::CreateDynamicImage
        {
            Atmos::Scripting::JavaScript::ImageAsset{ imageAsset.ID(), imageAssetName },
            0,
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>(),
            Render::Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            0.0f
        };

        const auto createTraits = "Atmos.Traits.Render.CreateDynamicImage";
        const auto traits = "Atmos.Traits.Render.DynamicImage";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyDynamicImage";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::DynamicImage image;
            Inscription::Json::FromString(image, json);

            REQUIRE(image.id > 0);

            const auto batch = reliquary.Batch<Atmos::Render::DynamicImage>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::DynamicText, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateDynamicText> Scenario(Arca::Reliquary& reliquary)
    {
        const auto fontAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>{
            Atmos::Buffer{}, fontAssetName });
        const auto fontAsset = reliquary.Do(Arca::Create<Atmos::Asset::Font>{
            fontAssetName, std::move(resource) });

        const auto createCommand = Atmos::Scripting::JavaScript::CreateDynamicText
        {
            "",
            Atmos::Scripting::JavaScript::FontAsset{ fontAsset.ID(), fontAssetName },
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>(),
            0.0f,
            false,
            false,
            Render::Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            0.0f
        };

        const auto createTraits = "Atmos.Traits.Render.CreateDynamicText";
        const auto traits = "Atmos.Traits.Render.DynamicText";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyDynamicText";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::DynamicText text;
            Inscription::Json::FromString(text, json);

            REQUIRE(text.id > 0);

            const auto batch = reliquary.Batch<Atmos::Render::DynamicText>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::GridRegion, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateGridRegion> Scenario(Arca::Reliquary& reliquary)
    {
        const auto points = std::unordered_set<Spatial::Grid::Point>
        {
            dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>()
        };

        const auto createCommand = Atmos::Scripting::JavaScript::CreateGridRegion
        {
            points,
            0,
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>()
        };

        const auto createTraits = "Atmos.Traits.Render.CreateGridRegion";
        const auto traits = "Atmos.Traits.Render.GridRegion";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyGridRegion";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::GridRegion region;
            Inscription::Json::FromString(region, json);

            REQUIRE(region.id > 0);

            const auto batch = reliquary.Batch<Atmos::Render::GridRegion>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::Line, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateLine> Scenario(Arca::Reliquary& reliquary)
    {
        const auto points = std::vector<Spatial::Point2D>
        {
            dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>()
        };

        const auto createCommand = Atmos::Scripting::JavaScript::CreateLine
        {
            points,
            0.0f,
            std::optional<Atmos::Scripting::JavaScript::MaterialAsset>()
        };

        const auto createTraits = "Atmos.Traits.Render.CreateLine";
        const auto traits = "Atmos.Traits.Render.Line";
        const auto destroyTraits = "Atmos.Traits.Render.DestroyLine";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::Line line;
            Inscription::Json::FromString(line, json);

            REQUIRE(line.id > 0);

            const auto batch = reliquary.Batch<Atmos::Render::Line>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Scripting::Script, T>, int> = 0>
    ScenarioT<Atmos::Scripting::JavaScript::CreateScript> Scenario(Arca::Reliquary& reliquary)
    {
        const auto assetName = dataGeneration.Random<String>();

        const auto source = "const executeMe = () => {};";
        Inscription::Plaintext::ToFile(source, "test.ts");
        const auto modules = std::vector<Scripting::Module>
        {
            { "test", source }
        };
        const auto compiledModules = reliquary.Do(Scripting::Compile{ modules });
        auto assetResource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Script>{ compiledModules[0].source, assetName });

        const auto asset = reliquary.Do(Arca::Create<Asset::Script>{assetName, std::move(assetResource)});
        
        const auto createCommand = Atmos::Scripting::JavaScript::CreateScript
        {
            Atmos::Scripting::JavaScript::ScriptAsset{ asset.ID(), assetName },
            "executeMe",
            Scripting::Parameters{}
        };

        const auto createTraits = "Atmos.Traits.Scripting.CreateScript";
        const auto traits = "Atmos.Traits.Scripting.Script";
        const auto destroyTraits = "Atmos.Traits.Scripting.DestroyScript";

        const std::function<void(const String&, Arca::Reliquary&)> expectations = [](const String& json, Arca::Reliquary& reliquary)
        {
            Scripting::JavaScript::Script script;
            Inscription::Json::FromString(script, json);

            REQUIRE(script.id > 0);

            const auto batch = reliquary.Batch<Atmos::Scripting::Script>();
            REQUIRE(batch.IsEmpty());
        };

        return { createCommand, createTraits, traits, destroyTraits, expectations };
    }
};