#include "AngelScriptDiagnosticsStatistics.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include <angelscript.h>

#include "AngelScriptGridPoint.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Diagnostics::Statistics::Profile>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, double>,
                    &PullFromParameter<1, double>,
                    &PullFromParameter<2, double>>,
                { "double time", "double average", "double highest" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::time>("double", "time")
            .Property<&Type::average>("double", "average")
            .Property<&Type::highest>("double", "highest")
            .Actualize(engine, documentationManager);
    }

    double Registration<Diagnostics::Statistics::Profile>::Time(Type type)
    {
        return type.time;
    }

    double Registration<Diagnostics::Statistics::Profile>::Average(Type type)
    {
        return type.average;
    }

    double Registration<Diagnostics::Statistics::Profile>::Highest(Type type)
    {
        return type.highest;
    }

    void Registration<Diagnostics::Statistics>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&RelicCount>, "uint32", "RelicCount", {})
            .ConstMethod(&Management::Method<&ShardCount>, "uint32", "ShardCount", {})
            .ConstMethod(&Management::Method<&NextRelicID>, "Arca::RelicID", "NextRelicID", {})
            .ConstMethod(&Management::Method<&Script>, "Atmos::Diagnostics::Statistics::Profile", "Script", {})
            .ConstMethod(&Management::Method<&Render>, "Atmos::Diagnostics::Statistics::Profile", "Render", {})
            .ConstMethod(&Management::Method<&Frame>, "Atmos::Diagnostics::Statistics::Profile", "Frame", {})
            .ConstMethod(&Management::Method<&Idle>, "Atmos::Diagnostics::Statistics::Profile", "Idle", {})
            .ConstMethod(&Management::Method<&Misc>, "Atmos::Diagnostics::Statistics::Profile", "Misc", {})
            .ConstMethod(&Management::Method<&FramesPerSecond>, "int", "FramesPerSecond", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Diagnostics::Statistics>>::RegisterTo(engine, documentationManager);
    }

    size_t Registration<Diagnostics::Statistics>::RelicCount(Type type)
    {
        return RequiredValue(type)->relicCount;
    }

    size_t Registration<Diagnostics::Statistics>::ShardCount(Type type)
    {
        return RequiredValue(type)->shardCount;
    }

    Arca::RelicID Registration<Diagnostics::Statistics>::NextRelicID(Type type)
    {
        return RequiredValue(type)->nextRelicID;
    }
    
    Diagnostics::Statistics::Profile Registration<Diagnostics::Statistics>::Script(Type type)
    {
        return RequiredValue(type)->script;
    }

    Diagnostics::Statistics::Profile Registration<Diagnostics::Statistics>::Render(Type type)
    {
        return RequiredValue(type)->render;
    }

    Diagnostics::Statistics::Profile Registration<Diagnostics::Statistics>::Frame(Type type)
    {
        return RequiredValue(type)->frame;
    }

    Diagnostics::Statistics::Profile Registration<Diagnostics::Statistics>::Idle(Type type)
    {
        return RequiredValue(type)->idle;
    }

    Diagnostics::Statistics::Profile Registration<Diagnostics::Statistics>::Misc(Type type)
    {
        return RequiredValue(type)->misc;
    }

    int Registration<Diagnostics::Statistics>::FramesPerSecond(Type type)
    {
        return RequiredValue(type)->framesPerSecond;
    }
}
