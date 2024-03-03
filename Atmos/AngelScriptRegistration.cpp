#include "AngelScriptRegistration.h"

#include <scriptstdstring.h>
#include <scripthelper.h>
#include <scriptarray.h>
#include <scriptdictionary.h>

#include "AngelScriptRelicID.h"
#include "AngelScriptArcaType.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include "AngelScriptArcaAssign.h"

#include "AngelScriptVariant.h"
#include "AngelScriptDatum.h"
#include "AngelScriptStringUtility.h"

#include "AngelScriptAngle2D.h"
#include "AngelScriptAngle3D.h"
#include "AngelScriptPoint2D.h"
#include "AngelScriptPoint3D.h"
#include "AngelScriptGridPoint.h"
#include "AngelScriptScalers2D.h"
#include "AngelScriptScalers3D.h"
#include "AngelScriptSize2D.h"
#include "AngelScriptSize3D.h"
#include "AngelScriptAxisAlignedBox2D.h"
#include "AngelScriptAxisAlignedBox3D.h"
#include "AngelScriptSpatialAlgorithms.h"
#include "AngelScriptGridSpatialAlgorithms.h"
#include "AngelScriptBounds.h"

#include "AngelScriptStopwatch.h"

#include "AngelScriptAsset.h"
#include "AngelScriptImageAsset.h"
#include "AngelScriptShaderAsset.h"
#include "AngelScriptMaterialAsset.h"
#include "AngelScriptAudioAsset.h"
#include "AngelScriptScriptAsset.h"

#include "AngelScriptLoggingSeverity.h"
#include "AngelScriptLog.h"

#include "AngelScriptColor.h"
#include "AngelScriptImageCore.h"
#include "AngelScriptDynamicImage.h"
#include "AngelScriptCamera.h"

#include "AngelScriptEntity.h"

#include "AngelScriptScriptResult.h"
#include "AngelScriptScript.h"
#include "AngelScriptSuspendScript.h"
#include "AngelScriptScriptFinished.h"
#include "AngelScriptCurrentScript.h"

namespace Atmos::Scripting::Angel
{
    class RegistrationHandler
    {
    public:
        RegistrationHandler(asIScriptEngine& engine) : engine(&engine)
        {}

        template<class T>
        RegistrationHandler& Register()
        {
            Registration<T>::RegisterTo(*engine);
            return *this;
        }
    private:
        asIScriptEngine* engine;
    };

    void RegisterAll(asIScriptEngine& engine)
    {
        RegisterStdString(&engine);

        RegisterExceptionRoutines(&engine);

        RegisterScriptArray(&engine, true);
        RegisterScriptDictionary(&engine);

        RegistrationHandler(engine)
            .Register<RelicID>()
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

            .Register<Variant>()
            .Register<Datum>()
            .Register<StringUtility>()

            .Register<Angle2D>()
            .Register<Spatial::Angle3D>()
            .Register<Spatial::Point2D>()
            .Register<Spatial::Point3D>()
            .Register<Spatial::Grid::Point>()
            .Register<Spatial::Scalers2D>()
            .Register<Spatial::Scalers3D>()
            .Register<Spatial::Size2D>()
            .Register<Spatial::Size3D>()
            .Register<Spatial::AxisAlignedBox2D>()
            .Register<Spatial::AxisAlignedBox3D>()
            .Register<SpatialAlgorithms>()
            .Register<GridSpatialAlgorithms>()
            .Register<Spatial::Bounds>()
            .Register<Spatial::MoveBoundsBy>()
            .Register<Spatial::MoveBoundsDirection>()
            .Register<Spatial::MoveBoundsTo>()
            .Register<Spatial::RotateBounds>()
            .Register<Spatial::ScaleBounds>()
            .Register<Spatial::BoundsMoved>()
            .Register<Spatial::BoundsRotated>()
            .Register<Spatial::BoundsScaled>()

            .Register<Time::Stopwatch>()

            .Register<GenericAssetFindByName>()
            .Register<Asset::ImageSize>()
            .Register<Asset::ImageGridSize>()
            .Register<Asset::Image>()
            .Register<Asset::Shader>()
            .Register<Asset::MaterialType>()
            .Register<Asset::Material::Pass>()
            .Register<Asset::Material>()
            .Register<Asset::Audio>()
            .Register<Asset::Script>()

            .Register<LoggingSeverity>()
            .Register<Logging::Log>()

            .Register<Render::Color>()
            .Register<Render::ImageCore>()
            .Register<Render::DynamicImage>()
            .Register<Render::Camera>()
            .Register<ChangeImageAsset>()
            .Register<ChangeAssetIndex>()
            .Register<ChangeColor>()
            .Register<ChangeMaterialAsset>()

            .Register<Entity::Entity>()
            .Register<Entity::FindByName>()
            .Register<Entity::FindByPosition>()
            .Register<Entity::MoveTo>()
            .Register<Entity::ModifyData>()
            .Register<Entity::ModifyTags>()

            .Register<Result>()
            .Register<Script>()
            .Register<Finished>()
            .Register<Suspend>()
            .Register<ModifyData>()
            .Register<CurrentScript>();
    }
}