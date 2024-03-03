#include "AngelScriptPoint3D.h"

#include "SpatialAlgorithms.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::Point3D>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>,
                    &PullFromParameter<2, Type::Value>>,
                { "float x", "float y", "float z" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Add(&Management::Add)
            .Subtract(&Management::Subtract)
            .Property<&Type::x>("float", "x")
            .Property<&Type::y>("float", "y")
            .Property<&Type::z>("float", "z")
            .Actualize(engine);
    }
}