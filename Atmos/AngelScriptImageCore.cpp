#include "AngelScriptImageCore.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptCommand.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::ImageCore>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Asset>, "Atmos::Asset::Image", "Asset", {})
            .ConstMethod(&Management::Method<&AssetIndex>, "int", "AssetIndex", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::ImageCore>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::ImageCore>>::RegisterTo(engine, documentationManager);

        ArcaCreateShardRegistration<Type::ValueT>()
            .Constructor<>({})
            .Constructor<
                Arca::Index<Asset::Image>,
                Type::ValueT::Index>
            ({
                "Atmos::Asset::Image asset",
                "int assetIndex"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyShard<Type::ValueT>(engine, documentationManager);
    }

    Arca::Index<Asset::Image> Registration<Render::ImageCore>::Asset(Type type)
    {
        return RequiredValue(type)->asset;
    }

    Render::ImageCore::Index Registration<Render::ImageCore>::AssetIndex(Type type)
    {
        return RequiredValue(type)->assetIndex;
    }
    
    void Registration<ChangeImageAsset>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Arca::Index<Asset::Image>>>,
                { "Arca::RelicID id", "Atmos::Asset::Image to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Asset::Image", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeImageCore Registration<ChangeImageAsset>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, fromAngelScript.to, {} };
    }

    void Registration<ChangeAssetIndex>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, std::int32_t>>,
                { "Arca::RelicID id", "int to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("int", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeImageCore Registration<ChangeAssetIndex>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, fromAngelScript.to };
    }
}