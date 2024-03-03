#include "AngelScriptRegistration.h"

#include "NullScriptDocumentationManager.h"

#include <scriptstdstring.h>
#include <scripthelper.h>
#include <scriptarray.h>
#include <scriptdictionary.h>

#include "AngelScriptNumericLimits.h"

#include "AngelScriptRelicID.h"
#include "AngelScriptArcaType.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include "AngelScriptArcaAssign.h"
#include "AngelScriptOpenRelic.h"

#include "AngelScriptRange.h"
#include "AngelScriptVariant.h"
#include "AngelScriptProperty.h"
#include "AngelScriptStringUtility.h"
#include "AngelScriptDataCore.h"
#include "AngelScriptModifyProperties.h"

#include "AngelScriptMath.h"

#include "AngelScriptAngle2D.h"
#include "AngelScriptAngle3D.h"
#include "AngelScriptPoint2D.h"
#include "AngelScriptPoint3D.h"
#include "AngelScriptScreenPoint.h"
#include "AngelScriptGridPoint.h"
#include "AngelScriptScalers2D.h"
#include "AngelScriptScalers3D.h"
#include "AngelScriptSize2D.h"
#include "AngelScriptSize3D.h"
#include "AngelScriptGridSize.h"
#include "AngelScriptAxisAlignedBox2D.h"
#include "AngelScriptAxisAlignedBox3D.h"
#include "AngelScriptGridAxisAlignedBox.h"
#include "AngelScriptSpatialAlgorithms.h"
#include "AngelScriptGridSpatialAlgorithms.h"
#include "AngelScriptToWorldPoint2D.h"
#include "AngelScriptToWorldPoint3D.h"
#include "AngelScriptBounds.h"

#include "AngelScriptTimePoint.h"
#include "AngelScriptStopwatch.h"

#include "AngelScriptKey.h"
#include "AngelScriptInputInformation.h"
#include "AngelScriptBindAction.h"
#include "AngelScriptActionActive.h"
#include "AngelScriptActionDepressed.h"
#include "AngelScriptActionPressed.h"
#include "AngelScriptMouseMoved.h"
#include "AngelScriptTextEntered.h"

#include "AngelScriptAsset.h"
#include "AngelScriptActionAsset.h"
#include "AngelScriptImageAsset.h"
#include "AngelScriptShaderAsset.h"
#include "AngelScriptMaterialAsset.h"
#include "AngelScriptAudioAsset.h"
#include "AngelScriptScriptAsset.h"

#include "AngelScriptLoggingSeverity.h"
#include "AngelScriptLog.h"

#include "AngelScriptPositionedSound.h"
#include "AngelScriptUniversalSound.h"
#include "AngelScriptChangeSoundVolume.h"
#include "AngelScriptRestartSound.h"
#include "AngelScriptPauseSound.h"
#include "AngelScriptResumeSound.h"
#include "AngelScriptSoundFinished.h"

#include "AngelScriptColor.h"
#include "AngelScriptImageCore.h"
#include "AngelScriptDynamicImage.h"
#include "AngelScriptRelativeImage.h"
#include "AngelScriptLine.h"
#include "AngelScriptGridRegion.h"
#include "AngelScriptCamera.h"
#include "AngelScriptChangeMaterialAsset.h"
#include "AngelScriptFindByBox.h"

#include "AngelScriptEntity.h"
#include "AngelScriptCurrentEntity.h"

#include "AngelScriptScriptResult.h"
#include "AngelScriptScript.h"
#include "AngelScriptSuspendScript.h"
#include "AngelScriptScriptFinished.h"
#include "AngelScriptCurrentScript.h"

#include "AngelScriptRequestField.h"
#include "AngelScriptModifyWorldProperties.h"
#include "AngelScriptRetrieveWorldProperties.h"

#include "AngelScriptGenerateRandomNumber.h"

namespace Atmos::Scripting::Angel
{
    class RegistrationHandler
    {
    public:
        RegistrationHandler(asIScriptEngine& engine, DocumentationManager& documentationManager) :
            engine(&engine), documentationManager(&documentationManager)
        {}

        template<class T>
        RegistrationHandler& Register()
        {
            Registration<T>::RegisterTo(*engine, *documentationManager);
            return *this;
        }
    private:
        asIScriptEngine* engine;
        DocumentationManager* documentationManager;
    };

    void RegisterAll(asIScriptEngine& engine)
    {
        NullDocumentationManager documentationManager;
        RegisterAll(engine, documentationManager);
    }

    void RegisterAll(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        RegisterStdString(&engine);

        RegisterExceptionRoutines(&engine);

        RegisterScriptArray(&engine, true);
        RegisterScriptDictionary(&engine);

        {
            RegistrationHandler(engine, documentationManager)
                .Register<std::numeric_limits<std::int8_t>>()
                .Register<std::numeric_limits<std::int16_t>>()
                .Register<std::numeric_limits<std::int32_t>>()
                .Register<std::numeric_limits<std::int64_t>>()
                .Register<std::numeric_limits<std::uint8_t>>()
                .Register<std::numeric_limits<std::uint16_t>>()
                .Register<std::numeric_limits<std::uint32_t>>()
                .Register<std::numeric_limits<std::uint64_t>>()
                .Register<std::numeric_limits<float>>()
                .Register<std::numeric_limits<double>>()

                .Register<RelicID>()
                .Register<NullRelicID>()
                .Register<Arca::Type>()
                .Register<Arca::Handle>()
                .Register<GenericArcaTraits>()
                .Register<GenericArcaBatch>()
                .Register<GenericArcaCreate>()
                .Register<GenericArcaCreateChild>()
                .Register<GenericArcaIdentifiedCreate>()
                .Register<GenericArcaIdentifiedCreateChild>()
                .Register<GenericArcaDestroy>()
                .Register<GenericArcaAssignCopy>()
                .Register<GenericArcaAssignMove>()
                .Register<Arca::Created>()
                .Register<GenericArcaCreatedKnown>()
                .Register<Arca::Destroying>()
                .Register<GenericArcaDestroyingKnown>()
                .Register<Arca::OpenRelic>()

                .Register<Range<std::int8_t>>()
                .Register<Range<std::int16_t>>()
                .Register<Range<std::int32_t>>()
                .Register<Range<std::int64_t>>()
                .Register<Range<std::uint8_t>>()
                .Register<Range<std::uint16_t>>()
                .Register<Range<std::uint32_t>>()
                .Register<Range<std::uint64_t>>()
                .Register<Range<float>>()
                .Register<Range<double>>()
                .Register<Variant>()
                .Register<Property>()
                .Register<StringUtility>()
                .Register<DataCore>()
                .Register<ModifyProperties>()

                .Register<Math>()

                .Register<Angle2D>()
                .Register<Spatial::Angle3D>()
                .Register<Spatial::Point2D>()
                .Register<Spatial::Point3D>()
                .Register<Spatial::ScreenPoint>()
                .Register<Spatial::Grid::Point>()
                .Register<Spatial::Scalers2D>()
                .Register<Spatial::Scalers3D>()
                .Register<Spatial::Size2D>()
                .Register<Spatial::Size3D>()
                .Register<Spatial::Grid::Size>()
                .Register<Spatial::AxisAlignedBox2D>()
                .Register<Spatial::AxisAlignedBox3D>()
                .Register<Spatial::Grid::AxisAlignedBox>()
                .Register<SpatialAlgorithms>()
                .Register<GridSpatialAlgorithms>()
                .Register<Spatial::ToWorldPoint2D>()
                .Register<Spatial::ToWorldPoint3D>()
                .Register<Spatial::Bounds>()
                .Register<Spatial::MoveBoundsBy>()
                .Register<Spatial::MoveBoundsDirection>()
                .Register<Spatial::MoveBoundsTo>()
                .Register<Spatial::RotateBounds>()
                .Register<Spatial::ScaleBounds>()
                .Register<Spatial::BoundsMoved>()
                .Register<Spatial::BoundsRotated>()
                .Register<Spatial::BoundsScaled>()

                .Register<Time::Hours>()
                .Register<Time::Minutes>()
                .Register<Time::Seconds>()
                .Register<Time::Milliseconds>()
                .Register<Time::Microseconds>()
                .Register<Time::Nanoseconds>()
                .Register<GenericTimePoint>()
                .Register<Time::Point<Time::Hours>>()
                .Register<Time::Point<Time::Minutes>>()
                .Register<Time::Point<Time::Seconds>>()
                .Register<Time::Point<Time::Milliseconds>>()
                .Register<Time::Point<Time::Microseconds>>()
                .Register<Time::Point<Time::Nanoseconds>>()
                .Register<TimeDurationCast<Time::Hours>>()
                .Register<TimeDurationCast<Time::Minutes>>()
                .Register<TimeDurationCast<Time::Seconds>>()
                .Register<TimeDurationCast<Time::Milliseconds>>()
                .Register<TimeDurationCast<Time::Microseconds>>()
                .Register<TimeDurationCast<Time::Nanoseconds>>()
                .Register<TimePointCast<Time::Hours>>()
                .Register<TimePointCast<Time::Minutes>>()
                .Register<TimePointCast<Time::Seconds>>()
                .Register<TimePointCast<Time::Milliseconds>>()
                .Register<TimePointCast<Time::Microseconds>>()
                .Register<TimePointCast<Time::Nanoseconds>>()
                .Register<Time::Stopwatch>()

                .Register<Input::Key>()
                .Register<Input::Information>()

                .Register<GenericAssetFindByName>()
                .Register<Asset::Action>()
                .Register<Asset::ImageSize>()
                .Register<Asset::ImageGridSize>()
                .Register<Asset::Image>()
                .Register<Asset::Shader>()
                .Register<Asset::Material::Pass>()
                .Register<Asset::ImageMaterial>()
                .Register<Asset::LineMaterial>()
                .Register<Asset::RegionMaterial>()
                .Register<Asset::Audio>()
                .Register<Asset::Script>()

                .Register<Input::BindAction>()
                .Register<Input::ActionActive>()
                .Register<Input::ActionDepressed>()
                .Register<Input::ActionPressed>()
                .Register<Input::MouseMoved>()
                .Register<Input::TextEntered>()

                .Register<Logging::Severity>()
                .Register<Logging::Log>()

                .Register<Audio::PositionedSound>()
                .Register<Audio::UniversalSound>()
                .Register<Audio::ChangeSoundVolume>()
                .Register<Audio::RestartSound>()
                .Register<Audio::PauseSound>()
                .Register<Audio::ResumeSound>()
                .Register<Audio::SoundFinished>()

                .Register<Render::Color>()
                .Register<Render::ImageCore>()
                .Register<Render::DynamicImage>()
                .Register<Render::RelativeImage>()
                .Register<Render::Line>()
                .Register<Render::GridRegion>()
                .Register<Render::Camera>()
                .Register<ChangeImageAsset>()
                .Register<ChangeAssetIndex>()
                .Register<Render::ChangeColor>()
                .Register<ChangeLinePoints>()
                .Register<ChangeLineZ>()
                .Register<ChangeGridRegionPoints>()
                .Register<ChangeGridRegionZ>()
                .Register<Render::ChangeMaterialAsset<Asset::ImageMaterial>>()
                .Register<Render::ChangeMaterialAsset<Asset::LineMaterial>>()
                .Register<Render::ChangeMaterialAsset<Asset::RegionMaterial>>()
                .Register<Render::FindImagesByBox>()
                .Register<Render::FindLinesByBox>()
                .Register<Render::FindGridRegionsByBox>()

                .Register<Entity::Entity>()
                .Register<Entity::FindByName>()
                .Register<Entity::FindByPosition>()
                .Register<Entity::MoveTo>()
                .Register<Entity::CanMoveTo>()
                .Register<Entity::ModifyTags>()
                .Register<Entity::FindPath>()
                .Register<CurrentEntity>()

                .Register<Result>()
                .Register<Script>()
                .Register<Finished>()
                .Register<Suspend>()
                .Register<CurrentScript>()

                .Register<World::RequestField>()
                .Register<World::ModifyProperties>()
                .Register<World::RetrieveProperties>()

                .Register<Random::GenerateBool>()
                .Register<Random::GenerateNumber<std::int8_t>>()
                .Register<Random::GenerateNumber<std::int16_t>>()
                .Register<Random::GenerateNumber<std::int32_t>>()
                .Register<Random::GenerateNumber<std::int64_t>>()
                .Register<Random::GenerateNumber<std::uint8_t>>()
                .Register<Random::GenerateNumber<std::uint16_t>>()
                .Register<Random::GenerateNumber<std::uint32_t>>()
                .Register<Random::GenerateNumber<std::uint64_t>>()
                .Register<Random::GenerateNumber<float>>()
                .Register<Random::GenerateNumber<double>>();
        }
    }
}