#include "AngelScriptHandle.h"

#include <angelscript.h>
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Arca::Handle>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        EnumRegistration<Arca::HandleObjectType>(ContainingNamespace(), "HandleObjectType")
            .Value("Relic", Arca::HandleObjectType::Relic)
            .Value("Shard", Arca::HandleObjectType::Shard)
            .Actualize(engine, documentationManager);

        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &UserData::RequiredReliquaryFrom,
                    &PullFromParameter<1, Arca::Type>,
                    &PullFromParameter<2, Arca::HandleObjectType>>,
                { "Arca::RelicID id", "Arca::Type type", "Arca::HandleObjectType objectType" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .ConstMethod(&Management::Method<&Type::ID>, "Arca::RelicID", "ID", {})
            .ConstMethod(&Management::Method<&Type::Type>, "Arca::Type", "Type", {})
            .ConstMethod(&Management::Method<&Type::ObjectType>, "Arca::HandleObjectType", "ObjectType", {})
            .Actualize(engine, documentationManager);
    }
}