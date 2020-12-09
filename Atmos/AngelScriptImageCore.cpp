#include "AngelScriptImageCore.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptCommand.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::ImageCore>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Asset>, "Atmos::Asset::Image", "Asset", {})
            .ConstMethod(&Management::Method<&AssetIndex>, "int", "AssetIndex", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::Material", "Material", {})
            .ConstMethod(&Management::Method<&Color>, "Atmos::Render::Color", "Color", {})
            .Actualize(engine);

        Registration<ArcaTraits<Render::ImageCore>>::RegisterTo(engine);
        Registration<Arca::Batch<Render::ImageCore>>::RegisterTo(engine);
    }

    Arca::Index<Asset::Image> Registration<Render::ImageCore>::Asset(Type type)
    {
        return RequiredValue(type)->asset;
    }

    Render::ImageCore::Index Registration<Render::ImageCore>::AssetIndex(Type type)
    {
        return RequiredValue(type)->assetIndex;
    }

    Arca::Index<Asset::Material> Registration<Render::ImageCore>::Material(Type type)
    {
        return RequiredValue(type)->material;
    }

    Render::Color Registration<Render::ImageCore>::Color(Type type)
    {
        return RequiredValue(type)->color;
    }

    void Registration<ChangeImageAsset>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
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
            .Actualize(engine);

        RegisterCommandHandler<&ToArca>(engine);
    }

    Render::ChangeImageCore Registration<ChangeImageAsset>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, IsRequired(fromAngelScript.to), {}, {}, {} };
    }

    void Registration<ChangeAssetIndex>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
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
            .Actualize(engine);

        RegisterCommandHandler<&ToArca>(engine);
    }

    Render::ChangeImageCore Registration<ChangeAssetIndex>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, fromAngelScript.to, {}, {} };
    }

    void Registration<ChangeColor>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
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
            .Actualize(engine);

        RegisterCommandHandler<&ToArca>(engine);
    }

    Render::ChangeImageCore Registration<ChangeColor>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, {}, fromAngelScript.to, {} };
    }

    void Registration<ChangeMaterialAsset>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Arca::Index<Asset::Material>>>,
                { "Arca::RelicID id", "Atmos::Asset::Material to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Asset::Material", "to")
            .Actualize(engine);

        RegisterCommandHandler<&ToArca>(engine);
    }

    Render::ChangeImageCore Registration<ChangeMaterialAsset>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, {}, {}, { IsRequired(fromAngelScript.to) } };
    }
}