
#include "FalconScriptObjects.h"
#include <falcon/engine.h>

namespace Atmos
{
    namespace Fal
    {
#define CLASS_NAME(classT, name)    \
const Name classT::className(name);

#define PROPERTY(classT, T, name)   \
classT::Property<T> classT::name(#name);

#define METHOD(classT, methodName, falconMethodName, isStatic)      \
    classT::Method classT::methodName(falconMethodName, isStatic,

#define PARAMETERS(...)         \
    std::vector<PropertyBase*>{

#define BODY(classT)                                                \
}, static_cast<classT::Method::FalconFuncT>([](Falcon::VMachine *vm)

#define BODY_END \
))

#define CONSTRUCTOR(classT)                 \
classT::Constructor classT::constructor(

        CLASS_NAME(GridPosition, "GridPosition");
        PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, x);
        PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, y);
        PROPERTY(GridPosition, ::Atmos::GridPosition::ValueT, z);
        // Distance method
        METHOD(GridPosition, distance, "Distance", false)
        PARAMETERS(new Property<GridPosition>("other"))
        BODY(GridPosition)
        {
            if (!distance.Setup())
                return;

            auto &self = *distance.self;
            auto &other = static_cast<Property<GridPosition>*>(distance.parameters.Get("other")->get())->value;
            vm->retval(
                std::abs(other.GetProperty(x)->value - self.GetProperty(x)->value) +
                std::abs(other.GetProperty(y)->value - self.GetProperty(y)->value) +
                std::abs(other.GetProperty(z)->value - self.GetProperty(z)->value));
        } BODY_END;

        // GridPosition constructor
        CONSTRUCTOR(GridPosition)
        PARAMETERS(new Property<::Atmos::GridPosition::ValueT>("x"), new Property<::Atmos::GridPosition::ValueT>("y"), new Property<::Atmos::GridPosition::ValueT>("z"))
        BODY(GridPosition)
        {
            constructor.SetAllPropertiesToParameters(*vm);
        } BODY_END;

        // FrameTimer constructor
        CONSTRUCTOR(FrameTimer)
        PARAMETERS()
        BODY(FrameTimer)
        {
            constructor.SetAllPropertiesToParameters(*vm);
        } BODY_END;

        // Modulator constructor
        CONSTRUCTOR(Modulator)
        PARAMETERS()
        BODY(Modulator)
        {
            constructor.SetAllPropertiesToParameters(*vm);
        } BODY_END;

        // ModulatorTrack constructor
        CONSTRUCTOR(ModulatorTrack)
        PARAMETERS()
        BODY(ModulatorTrack)
        {
            constructor.SetAllPropertiesToParameters(*vm);
        } BODY_END;

        // ModulatorTrackNode constructor
        CONSTRUCTOR(ModulatorTrackNode)
        PARAMETERS()
        BODY(ModulatorTrackNode)
        {
            constructor.SetAllPropertiesToParameters(*vm);
        } BODY_END;

        /*
        GridPosition::Method GridPosition::distance("Distance", false,
        std::vector<PropertyBase*>{ new Property<GridPosition>("other") },
        static_cast<GridPosition::Method::FalconFuncT>([](Falcon::VMachine *vm)
        {
        if (!distance.Setup())
        return;

        auto &self = *distance.self;
        auto &other = distance.parameters.Get<GridPosition>("other");
        vm->retval(
        std::abs(other.GetProperty(x)->value - self.GetProperty(x)->value) +
        std::abs(other.GetProperty(y)->value - self.GetProperty(y)->value) +
        std::abs(other.GetProperty(z)->value - self.GetProperty(z)->value));
        }));
        */
    }
}