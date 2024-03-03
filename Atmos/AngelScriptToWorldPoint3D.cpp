#include "AngelScriptToWorldPoint3D.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptPoint3D.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::ToWorldPoint3D>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Spatial::ScreenPoint>,
                    &PullFromParameter<1, Spatial::Point3D::Value>>,
                { "Atmos::Spatial::ScreenPoint from", "float z" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::from>("Atmos::Spatial::ScreenPoint", "from")
            .Property<&Type::z>("float", "z")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    Spatial::Point3D Registration<Spatial::ToWorldPoint3D>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }
}