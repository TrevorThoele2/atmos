#include "AngelScriptColor.h"

#include "AngelScriptCommand.h"

#include <angelscript.h>
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::Color>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Render::Color>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>,
                    &PullFromParameter<2, Type::Value>,
                    &PullFromParameter<3, Type::Value>>,
                { "uint8 alpha", "uint8 red", "uint8 green", "uint8 blue" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::alpha>("uint8", "alpha")
            .Property<&Type::red>("uint8", "red")
            .Property<&Type::green>("uint8", "green")
            .Property<&Type::blue>("uint8", "blue")
            .Actualize(engine, documentationManager);
    }

    void Registration<Render::ChangeColor>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Render::Color>>,
                { "Arca::RelicID id", "Atmos::Render::Color to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Render::Color", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}