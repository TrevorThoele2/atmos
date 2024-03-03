#include "AngelScriptChangeMaterialAsset.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Render::ChangeMaterialAsset>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::template GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, Arca::Index<Asset::Material>>>,
                { "Arca::RelicID id", "Atmos::Asset::Material to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Asset::Material", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}