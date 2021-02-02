#include "AngelScriptEntity.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Entity::Entity>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&DoName>, "string", "Name", {})
            .ConstMethod(&Management::Method<&DisplayName>, "string", "DisplayName", {})
            .ConstMethod(&Management::Method<&Position>, "Atmos::Spatial::Grid::Point", "Position", {})
            .ConstMethod(&Management::Method<&Direction>, "Atmos::Spatial::Angle2D", "Direction", {})
            .ConstMethod(&Management::Method<&IsSolid>, "bool", "IsSolid", {})
            .ConstMethod(&Management::Method<&Tags>, "string[]@", "Tags", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Entity::Entity>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Entity::Entity>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                Atmos::Name,
                Atmos::Name,
                Spatial::Grid::Point,
                Spatial::Angle2D,
                bool>
            ({
                "string name",
                "string displayName",
                "Atmos::Spatial::Grid::Point position",
                "Atmos::Spatial::Angle2D direction",
                "bool isSolid"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }

    String Registration<Entity::Entity>::DoName(Type type)
    {
        return RequiredValue(type)->name;
    }

    String Registration<Entity::Entity>::DisplayName(Type type)
    {
        return RequiredValue(type)->displayName;
    }

    Spatial::Grid::Point Registration<Entity::Entity>::Position(Type type)
    {
        return RequiredValue(type)->position;
    }

    Spatial::Angle2D Registration<Entity::Entity>::Direction(Type type)
    {
        return RequiredValue(type)->direction;
    }

    bool Registration<Entity::Entity>::IsSolid(Type type)
    {
        return RequiredValue(type)->isSolid;
    }

    std::vector<String> Registration<Entity::Entity>::Tags(Type type)
    {
        return RequiredValue(type)->tags;
    }

    void Registration<Entity::FindByName>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, String>>,
                { "string name" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::name>("string", "name")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    Arca::Index<Entity::Entity> Registration<Entity::FindByName>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }

    void Registration<Entity::FindByPosition>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Spatial::Grid::Point>>,
                { "Atmos::Spatial::Grid::Point" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::position>("Atmos::Spatial::Grid::Point", "position")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    std::vector<Arca::Index<Entity::Entity>> Registration<Entity::FindByPosition>::ToCommandReturn(
        Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return { fromArca.begin(), fromArca.end() };
    }

    void Registration<Entity::MoveTo>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::Index<Entity::Entity>>,
                    &PullFromParameter<1, Spatial::Grid::Point>>,
                { "Atmos::Entity::Entity entity", "Atmos::Spatial::Grid::Point to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::entity>("Atmos::Entity::Entity", "entity")
            .Property<&Type::to>("Atmos::Spatial::Grid::Point", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Entity::ModifyTags>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::Index<Entity::Entity>>,
                    &PullFromParameter<1, std::vector<String>>,
                    &PullFromParameter<2, std::vector<String>>>,
                { "Atmos::Entity::Entity entity", "string[]@ add", "string[]@ remove" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::entity>("Atmos::Entity::Entity", "entity")
            .Property<&Type::add>("string[]@", "add")
            .Property<&Type::remove>("string[]@", "remove")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}
