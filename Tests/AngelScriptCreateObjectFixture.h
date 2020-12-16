#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/AngelScriptAudioAsset.h>
#include <Atmos/AngelScriptImageAsset.h>
#include <Atmos/AngelScriptShaderAsset.h>
#include <Atmos/AngelScriptMaterialAsset.h>
#include <Atmos/AngelScriptScriptAsset.h>
#include <Atmos/AngelScriptDynamicImage.h>
#include <Atmos/AngelScriptRelativeImage.h>
#include <Atmos/AngelScriptLine.h>
#include <Atmos/AngelScriptGridRegion.h>
#include <Atmos/AngelScriptEntity.h>
#include <Atmos/AngelScriptScript.h>

#include <Atmos/AngelScriptImageCore.h>
#include <Atmos/AngelScriptBounds.h>

#include <Atmos/CreateAudioAssetResource.h>
#include "AudioBuffer.h"

#include <Arca/LocalRelic.h>

class AngelScriptCreateObjectFixture : public AngelScriptFixture
{
public:
    template<class T>
    using CreatedObject = std::tuple<Arca::Index<T>, Atmos::String>;
public:
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
        const auto width = dataGeneration.Random<Atmos::Asset::ImageSize::Dimension>(
            TestFramework::Range<Atmos::Asset::ImageSize::Dimension>{1, std::numeric_limits<Atmos::Asset::ImageSize::Dimension>::max()});
        const auto height = dataGeneration.Random<Atmos::Asset::ImageSize::Dimension>(
            TestFramework::Range<Atmos::Asset::ImageSize::Dimension>{1, std::numeric_limits<Atmos::Asset::ImageSize::Dimension>::max()});
        const auto columns = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();
        const auto rows = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::DataBuffer{}, name, Atmos::Asset::ImageSize{ width, height }});
        const auto index = reliquary.Do(Arca::Create<Atmos::Asset::Image>{ name, std::move(resource), Atmos::Asset::ImageGridSize{ columns, rows } });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::Shader, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::DataBuffer{}, name});
        const auto index = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ name, std::move(resource) });
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::ImageMaterial, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        return CreateMaterialAsset<Atmos::Asset::ImageMaterial>(reliquary);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::LineMaterial, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        return CreateMaterialAsset<Atmos::Asset::LineMaterial>(reliquary);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Asset::RegionMaterial, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        return CreateMaterialAsset<Atmos::Asset::RegionMaterial>(reliquary);
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
            Atmos::DataBuffer{}, imageAssetName, Atmos::Asset::ImageSize{ 1, 1 }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(resource), Atmos::Asset::ImageGridSize{ 1, 1 } });

        const auto createCommand = Arca::Create<Atmos::Render::DynamicImage>{
            imageAsset,
            1,
            Arca::Index<Atmos::Asset::ImageMaterial>{},
            Atmos::Render::Color{},
            Atmos::Spatial::Point3D{},
            Atmos::Spatial::Scalers2D{},
            Atmos::Spatial::Angle2D{} };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::RelativeImage, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();

        auto resource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::DataBuffer{}, imageAssetName, Atmos::Asset::ImageSize{ 1, 1 }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(resource), Atmos::Asset::ImageGridSize{ 1, 1 } });
        
        const auto createCommand = Arca::Create<Atmos::Render::RelativeImage>{
            imageAsset,
            1,
            Arca::Index<Atmos::Asset::ImageMaterial>{},
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
            Arca::Index<Atmos::Asset::LineMaterial>{},
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
            Arca::Index<Atmos::Asset::RegionMaterial>{} };
        auto index = reliquary.Do(createCommand);
        return TupleOf<T>(index);
    }

    template<class T, std::enable_if_t<std::is_same_v<Atmos::Entity::Entity, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto displayName = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<
            Atmos::Spatial::Grid::Point, Atmos::Spatial::Grid::Point::Value, Atmos::Spatial::Grid::Point::Value>();
        const auto direction = dataGeneration.Random<Atmos::Spatial::Angle2D>();
        const auto isSolid = dataGeneration.Random<bool>();

        auto index = reliquary.Do(Arca::Create<Atmos::Entity::Entity>{ name, displayName, position, direction, isSolid });
        return TupleOf<T>(index);
    }
public:
    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::ImageCore, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto imageAssetName = dataGeneration.Random<std::string>();
        const auto imageAssetWidth = dataGeneration.Random<Atmos::Asset::ImageSize::Dimension>(
            TestFramework::Range<Atmos::Asset::ImageSize::Dimension>{1, std::numeric_limits<Atmos::Asset::ImageSize::Dimension>::max()});
        const auto imageAssetHeight = dataGeneration.Random<Atmos::Asset::ImageSize::Dimension>(
            TestFramework::Range<Atmos::Asset::ImageSize::Dimension>{1, std::numeric_limits<Atmos::Asset::ImageSize::Dimension>::max()});
        const auto imageAssetColumns = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();
        const auto imageAssetRows = dataGeneration.Random<Atmos::Asset::ImageGridSize::Dimension>();

        auto imageAssetResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
            Atmos::DataBuffer{}, imageAssetName, Atmos::Asset::ImageSize{ imageAssetWidth, imageAssetHeight }});
        const auto imageAsset = reliquary.Do(Arca::Create<Atmos::Asset::Image>{
            imageAssetName, std::move(imageAssetResource), Atmos::Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

        const auto vertexShaderName = dataGeneration.Random<std::string>();
        auto vertexResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::DataBuffer{}, vertexShaderName});
        const auto vertexShaderAsset = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

        const auto fragmentShaderName = dataGeneration.Random<std::string>();
        auto fragmentResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::DataBuffer{}, fragmentShaderName});
        const auto fragmentShaderAsset = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ fragmentShaderName, std::move(fragmentResource) });

        const auto materialAssetName = dataGeneration.Random<std::string>();
        const auto materialAssetPasses = std::vector<Atmos::Asset::Material::Pass>
        {
            { vertexShaderAsset, fragmentShaderAsset }
        };
        const auto materialAsset = reliquary.Do(Arca::Create<Atmos::Asset::ImageMaterial>{ materialAssetName, materialAssetPasses });

        const auto assetIndex = dataGeneration.Random<Atmos::Render::ImageCore::Index>();
        const auto color = dataGeneration.RandomStack<
            Atmos::Render::Color,
            Atmos::Render::Color::Value,
            Atmos::Render::Color::Value,
            Atmos::Render::Color::Value,
            Atmos::Render::Color::Value>();

        const auto openRelic = reliquary.Do(Arca::Create<Arca::OpenRelic>());

        const auto index = reliquary.Do(Arca::Create<Atmos::Render::ImageCore>{ openRelic, imageAsset, assetIndex, materialAsset, color });
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
        const auto index = reliquary.Do(Arca::Create<Atmos::Spatial::Bounds>{openRelic, position, baseSize, scalers, rotation});
        return TupleOf<T>(index);
    }
private:
    template<class T>
    CreatedObject<T> TupleOf(Arca::Index<T> index)
    {
        using Registration = Atmos::Scripting::Angel::Registration<T>;
        return { index, Atmos::Scripting::Angel::CreateName({Registration::containingNamespace}, Registration::name) };
    }

    template<class T>
    CreatedObject<T> CreateMaterialAsset(Arca::Reliquary& reliquary)
    {
        const auto vertexShaderName = dataGeneration.Random<std::string>();
        auto vertexResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::DataBuffer{}, vertexShaderName});
        const auto vertexShaderAsset = reliquary.Do(Arca::Create<Atmos::Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

        const auto fragmentShaderName = dataGeneration.Random<std::string>();
        auto fragmentResource = reliquary.Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Shader>{Atmos::DataBuffer{}, fragmentShaderName});
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