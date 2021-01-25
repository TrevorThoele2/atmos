#include "AngelScriptGenerateRandomNumber.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Random::GenerateBool>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    bool Registration<Random::GenerateBool>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }
}