#include "AngelScriptScriptResult.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Result>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .CopyAssignment(&Management::CopyAssign)
            .Destructor(&Management::DestructValue)
            .ConstMethod(&Management::Method<&AsVariant>, "Atmos::Variant", "AsVariant", {})
            .ConstMethod(&Management::Method<&IsVariant>, "bool", "IsVariant", {})
            .ConstMethod(&Management::Method<&IsFailure>, "bool", "IsFailure", {})
            .ConstMethod(&Management::Method<&IsQuit>, "bool", "IsQuit", {})
            .Actualize(engine, documentationManager);
    }

    Variant Registration<Result>::AsVariant(Type type)
    {
        return std::get<Variant>(type);
    }

    bool Registration<Result>::IsVariant(Type type)
    {
        return std::holds_alternative<Variant>(type);
    }

    bool Registration<Result>::IsFailure(Type type)
    {
        return std::holds_alternative<Failure>(type);
    }

    bool Registration<Result>::IsQuit(Type type)
    {
        return std::holds_alternative<Quit>(type);
    }
}