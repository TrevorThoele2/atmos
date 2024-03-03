#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/AngelScriptActionAsset.h>
#include <Atmos/AngelScriptAudioAsset.h>
#include <Atmos/AngelScriptImageAsset.h>
#include <Atmos/AngelScriptFontAsset.h>
#include <Atmos/AngelScriptShaderAsset.h>
#include <Atmos/AngelScriptMaterialAsset.h>
#include <Atmos/AngelScriptScriptAsset.h>
#include <Atmos/AngelScriptDynamicImage.h>
#include <Atmos/AngelScriptLine.h>
#include <Atmos/AngelScriptGridRegion.h>
#include <Atmos/AngelScriptUIImage.h>
#include <Atmos/AngelScriptEntity.h>
#include <Atmos/AngelScriptScript.h>

#include <Atmos/AngelScriptImageCore.h>
#include <Atmos/AngelScriptBounds.h>

#include <Atmos/AngelScriptPositionedSound.h>
#include <Atmos/AngelScriptUniversalSound.h>

#include <Atmos/CreateAudioAssetResource.h>
#include "AudioBuffer.h"

#include <Arca/OpenRelic.h>

class AngelScriptCreateObjectFixture : public AngelScriptFixture
{
public:
    template<class T>
    using CreatedObject = std::tuple<Arca::Index<T>, Atmos::String>;
public:
    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Action, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<Atmos::String>();
        const auto mappedKey = static_cast<Atmos::Input::Key>(dataGeneration.Random<std::underlying_type_t<Atmos::Input::Key>>());
        Atmos::Asset::Action::Modifiers mappedKeys = { mappedKey };

        const auto index = reliquary.Do(Arca::Create<T>{ name, mappedKeys });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Audio, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<Atmos::String>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>{ audioBuffer, name });

        const auto index = reliquary.Do(Arca::Create<T>{ name, std::move(resource) });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Image, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto width = dataGeneration.Random<Atmos::Spatial::Size2D::Value>(
            TestFramework::Range<Atmos::Spatial::Size2D::Value>{1, std::numeric_limits<Atmos::Spatial::Size2D::Value>::max()});
        const auto height = dataGeneration.Random<Atmos::Spatial::Size2D::Value>(
            TestFramework::Range<Atmos::Spatial::Size2D::Value>{1, std::numeric_limits<Atmos::Spatial::Size2D::Value>::max()});
        const auto columns = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();
        const auto rows = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::Buffer{}, name, Atmos::Spatial::Size2D{ width, height }});
        const auto index = reliquary.Do(Arca::Create<Atmos::Asset::Image>{ name, std::move(resource), Atmos::Asset::ImageGridSize{ columns, rows } });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Font, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>{
            Atmos::Buffer{}});
        const auto index = reliquary.Do(Arca::Create<Atmos::Asset::Font>{ name, std::move(resource) });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Shader, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::Buffer{}, name});
        const auto index = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ name, std::move(resource) });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Material, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        return CreateMaterialAsset<Atmos::Asset::Material>(reliquary);
    }
    
    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Script, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();

        const auto index = CompileAndCreateScriptAsset(name, "void main(){}", reliquary);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::DynamicImage, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::Buffer{}, imageAssetName, Atmos::Spatial::Size2D{ 1, 1 }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(resource), Atmos::Asset::ImageGridSize{ 1, 1 } });

        const auto createCommand = Arca::Create<Atmos::Render::DynamicImage>{
            imageAsset,
            1,
            Arca::Index<Atmos::Asset::Material>{},
            Atmos::Render::Color{},
            Atmos::Spatial::Point3D{},
            Atmos::Spatial::Scalers2D{},
            Atmos::Spatial::Angle2D{} };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::UI::Image, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::Buffer{}, imageAssetName, Atmos::Spatial::Size2D{ 1, 1 }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(resource), Atmos::Asset::ImageGridSize{ 1, 1 } });

        const auto createCommand = Arca::Create<Atmos::UI::Image>{
            imageAsset,
            1,
            Arca::Index<Atmos::Asset::Material>{},
            Atmos::Render::Color{},
            Atmos::Spatial::Point3D{},
            Atmos::Spatial::Scalers2D{},
            Atmos::Spatial::Angle2D{} };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::Line, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto points = dataGeneration.RandomStackGroup<
            Atmos::Spatial::Point2D, Atmos::Spatial::Point2D::Value, Atmos::Spatial::Point2D::Value>(3);
        const auto z = dataGeneration.Random<Atmos::Spatial::Point2D::Value>();
        const auto width = dataGeneration.Random<Atmos::Render::LineWidth>();
        const auto color = dataGeneration.RandomStack<
            Atmos::Render::Color, Atmos::Render::Color::Value, Atmos::Render::Color::Value, Atmos::Render::Color::Value>();

        const auto createCommand = Arca::Create<Atmos::Render::Line>{
            points,
            z,
            Arca::Index<Atmos::Asset::Material>{},
            width,
            color };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::GridRegion, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto pointsVector = dataGeneration.RandomStackGroup<
            Atmos::Spatial::Grid::Point, Atmos::Spatial::Grid::Point::Value, Atmos::Spatial::Grid::Point::Value>(3);
        const auto points = std::unordered_set<Atmos::Spatial::Grid::Point>{ pointsVector.begin(), pointsVector.end() };
        const auto z = dataGeneration.Random<Atmos::Spatial::Grid::Point::Value>();

        const auto createCommand = Arca::Create<Atmos::Render::GridRegion>{
            points,
            z,
            Arca::Index<Atmos::Asset::Material>{} };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Audio::PositionedSound, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>{
            Atmos::Buffer{}, audioAssetName });
        const auto audioAsset = reliquary.Do(Arca::Create<Atmos::Asset::Audio>{
            audioAssetName, std::move(resource) });

        const auto volume = dataGeneration.Random<Atmos::Audio::Volume>();

        const auto position = dataGeneration.RandomStack<
            Atmos::Spatial::Point3D, Atmos::Spatial::Point3D::Value, Atmos::Spatial::Point3D::Value, Atmos::Spatial::Point3D::Value>();

        const auto createCommand = Arca::Create<Atmos::Audio::PositionedSound>{
            audioAsset,
            volume,
            position };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Audio::UniversalSound, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>{
            Atmos::Buffer{}, audioAssetName });
        const auto audioAsset = reliquary.Do(Arca::Create<Atmos::Asset::Audio>{
            audioAssetName, std::move(resource) });

        const auto volume = dataGeneration.Random<Atmos::Audio::Volume>();

        const auto createCommand = Arca::Create<Atmos::Audio::UniversalSound>{
            audioAsset,
            volume };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Entity::Entity, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<
            Atmos::Spatial::Grid::Point, Atmos::Spatial::Grid::Point::Value, Atmos::Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();

        auto index = reliquary.Do(Arca::Create<Atmos::Entity::Entity>{ name, position, isSolid });
        return TupleOf<T>(index);
    }
public:
    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::ImageCore, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();
        const auto imageAssetWidth = dataGeneration.Random<Atmos::Spatial::Size2D::Value>(
            TestFramework::Range<Atmos::Spatial::Size2D::Value>{1, std::numeric_limits<Atmos::Spatial::Size2D::Value>::max()});
        const auto imageAssetHeight = dataGeneration.Random<Atmos::Spatial::Size2D::Value>(
            TestFramework::Range<Atmos::Spatial::Size2D::Value>{1, std::numeric_limits<Atmos::Spatial::Size2D::Value>::max()});
        const auto imageAssetColumns = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();
        const auto imageAssetRows = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();

        auto imageAssetResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::Buffer{}, imageAssetName, Atmos::Spatial::Size2D{ imageAssetWidth, imageAssetHeight }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(imageAssetResource), Atmos::Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });
        
        const auto assetIndex = dataGeneration.Random<Atmos::Render::ImageCore::Index>();

        const auto openRelic = reliquary.Do(Arca::Create<Arca::OpenRelic>());

        const auto index = reliquary.Do(Arca::Create<Atmos::Render::ImageCore>{ openRelic, imageAsset, assetIndex });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Spatial::Bounds, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto openRelic = reliquary.Do(Arca::Create<Arca::OpenRelic>());

        const auto position = dataGeneration.RandomStack<
            Atmos::Spatial::Point3D, Atmos::Spatial::Point3D::Value, Atmos::Spatial::Point3D::Value, Atmos::Spatial::Point3D::Value>();
        const auto baseSize = dataGeneration.RandomStack<
            Atmos::Spatial::Size2D, Atmos::Spatial::Size2D::Value, Atmos::Spatial::Size2D::Value>();
        const auto scalers = dataGeneration.RandomStack<
            Atmos::Spatial::Scalers2D, Atmos::Spatial::Scalers2D::Value, Atmos::Spatial::Scalers2D::Value>();
        const auto rotation = dataGeneration.Random<Atmos::Spatial::Angle2D>();
        const auto index = reliquary.Do(Arca::Create<Atmos::Spatial::Bounds>{
            openRelic, Atmos::Spatial::Space::World, position, baseSize, scalers, rotation});
        return TupleOf<T>(index);
    }
private:
    template<class T>
    CreatedObject<T> TupleOf(Arca::Index<T> index)
    {
        using Registration = Atmos::Scripting::Angel::Registration<T>;
        return { index, Atmos::Scripting::Angel::CreateName({Registration::ContainingNamespace()}, Registration::Name()) };
    }

    template<class T>
    CreatedObject<T> CreateMaterialAsset(Arca::Reliquary& reliquary)
    {
        const auto vertexShaderName = dataGeneration.Random<std::string>();
        auto vertexResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::Buffer{}, vertexShaderName});
        const auto vertexShaderAsset = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

        const auto fragmentShaderName = dataGeneration.Random<std::string>();
        auto fragmentResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::Buffer{}, fragmentShaderName});
        const auto fragmentShaderAsset = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ fragmentShaderName, std::move(fragmentResource) });

        const auto materialAssetName = dataGeneration.Random<std::string>();
        const auto materialAssetPasses = std::vector<Atmos::Asset::MaterialPass>
        {
            { vertexShaderAsset, fragmentShaderAsset }
        };
        const auto index = reliquary.Do(Arca::Create<T>{ materialAssetName, materialAssetPasses });
        return TupleOf<T>(index);
    }
};