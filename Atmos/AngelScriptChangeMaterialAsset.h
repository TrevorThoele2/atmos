#pragma once

#include "ChangeMaterialAsset.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"
#include "AngelScriptCommand.h"

#include "String.h"

#include <Chroma/Identity.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<class Material>
    struct ChangeMaterialAssetTraits;

    template<>
    struct ChangeMaterialAssetTraits<Asset::ImageMaterial>
    {
        static inline const String nameToken = "Image";
    };

    template<>
    struct ChangeMaterialAssetTraits<Asset::LineMaterial>
    {
        static inline const String nameToken = "Line";
    };

    template<>
    struct ChangeMaterialAssetTraits<Asset::RegionMaterial>
    {
        static inline const String nameToken = "Region";
    };

    template<class Material>
    struct Registration<Render::ChangeMaterialAsset<Material>>
    {
        using Type = Render::ChangeMaterialAsset<Material>;

        using Traits = ChangeMaterialAssetTraits<Material>;

        static String Name() { return "Change" + Traits::nameToken + "MaterialAsset"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class Material>
    void Registration<Render::ChangeMaterialAsset<Material>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::template GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Arca::Index<Material>>>,
                { "Arca::RelicID id", "Atmos::Asset::" + Traits::nameToken + "Material to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .template Property<&Type::id>("Arca::RelicID", "id")
            .template Property<&Type::to>("Atmos::Asset::" + Traits::nameToken + "Material", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}