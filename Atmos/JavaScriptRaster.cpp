#include "JavaScriptRaster.h"

#include "JavaScriptExecutingMaterial.h"
#include "JavaScriptRecordRasterCommands.h"

#include "JavaScriptTraits.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<RasterNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "ExecutingMaterial", ToV8(*isolate, ArcaTraits::From<Render::Raster::ExecutingMaterial>()) },
                { "RecordCommands", ToV8(*isolate, ArcaTraits::From<Render::Raster::RecordCommands>()) }
            });
    }
}
