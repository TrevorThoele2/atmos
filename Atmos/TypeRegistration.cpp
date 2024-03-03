#include "TypeRegistration.h"

#include "DebugStatistics.h"
#include "FocusLost.h"
#include "FocusRegained.h"
#include "TimeCurator.h"
#include "StaticMaterialView.h"
#include "InputManagerProvider.h"
#include "GraphicsManagerProvider.h"
#include "AudioManagerProvider.h"
#include "Camera.h"
#include "FrameStopwatch.h"
#include "MaterialViewCurator.h"

namespace Atmos
{
    void RegisterTypes(Arca::ReliquaryOrigin& origin)
    {
        origin
            .Type<FocusRegained>()
            .Type<FocusLost>()
            .Type<Bounds>()
            .Type<Debug::Statistics>()
            .Type<Time::TimeCurator>()
            .Type<Time::Information>()
            .Type<Time::Settings>()
            .Type<Time::RealStopwatch>()
            .Type<Time::FrameStopwatch>()
            .Type<Input::ManagerProvider>()
            .Type<Render::GraphicsManagerProvider>()
            .Type<Render::StaticMaterialView>()
            .Type<Render::Camera>()
            .Type<Render::MaterialViewCore>()
            .Type<Render::MaterialViewCurator>()
            .Type<Audio::AudioManagerProvider>();
    }
}
