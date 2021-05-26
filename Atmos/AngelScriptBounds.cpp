#include "AngelScriptBounds.h"

#include "AngelScriptEnumRegistration.h"

#include "AngelScriptCommand.h"
#include "AngelScriptSignal.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::BoundsSpace>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        EnumRegistration<Spatial::BoundsSpace>(ContainingNamespace(), Name())
            .Value("World", Spatial::BoundsSpace::World)
            .Value("Screen", Spatial::BoundsSpace::Screen)
            .Actualize(engine, documentationManager);
    }

    void Registration<Spatial::Bounds>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Position>, "Atmos::Spatial::Point3D", "Position", {})
            .ConstMethod(&Management::Method<&BaseSize>, "Atmos::Spatial::Size2D", "BaseSize", {})
            .ConstMethod(&Management::Method<&Size>, "Atmos::Spatial::Size2D", "Size", {})
            .ConstMethod(&Management::Method<&Scalers>, "Atmos::Spatial::Scalers2D", "Scalers", {})
            .ConstMethod(&Management::Method<&Rotation>, "Atmos::Spatial::Angle2D", "Rotation", {})
            .ConstMethod(&Management::Method<&Space>, "Atmos::Spatial::BoundsSpace", "Space", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Spatial::Bounds>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Spatial::Bounds>>::RegisterTo(engine, documentationManager);

        ArcaCreateShardRegistration<Type::ValueT>()
            .Constructor<>({})
            .Constructor<
                Spatial::BoundsSpace,
                Spatial::Point3D,
                Spatial::Size2D,
                Spatial::Scalers2D,
                Spatial::Angle2D>
            ({
                "Atmos::Spatial::BoundsSpace space",
                "Atmos::Spatial::Point3D position",
                "Atmos::Spatial::Size2D baseSize",
                "Atmos::Spatial::Scalers2D scalers",
                "Atmos::Spatial::Angle2D rotation"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyShard<Type::ValueT>(engine, documentationManager);
    }

    Spatial::Point3D Registration<Spatial::Bounds>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<Spatial::Bounds>::BaseSize(Type type)
    {
        return RequiredValue(type)->BaseSize();
    }

    Spatial::Size2D Registration<Spatial::Bounds>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::Scalers2D Registration<Spatial::Bounds>::Scalers(Type type)
    {
        return RequiredValue(type)->Scalers();
    }

    Spatial::Angle2D Registration<Spatial::Bounds>::Rotation(Type type)
    {
        return RequiredValue(type)->Rotation();
    }

    Spatial::BoundsSpace Registration<Spatial::Bounds>::Space(Type type)
    {
        return RequiredValue(type)->Space();
    }
    
    void Registration<Spatial::MoveBounds>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Point3D>>,
                { "Arca::RelicID id", "Atmos::Spatial::Point3D to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Spatial::Point3D", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Spatial::RotateBounds>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Angle2D>>,
                { "Arca::RelicID id", "Atmos::Spatial::Angle2D to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Spatial::Angle2D", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Spatial::ScaleBounds>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Scalers2D>>,
                { "Arca::RelicID id", "Atmos::Spatial::Scalers2D to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Spatial::Scalers2D", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Spatial::BoundsMoved>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Point3D>>,
                { "Arca::RelicID id", "Atmos::Spatial::Point3D previousPosition"})
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::previousPosition>("Atmos::Spatial::Point3D", "previousPosition")
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Spatial::BoundsRotated>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Angle2D>>,
                { "Arca::RelicID id", "Atmos::Spatial::Angle2D previousRotation"})
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::previousRotation>("Atmos::Spatial::Angle2D", "previousRotation")
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Spatial::BoundsScaled>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Scalers2D>>,
                { "Arca::RelicID id", "Atmos::Spatial::Scalers2D previousScalers"})
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::previousScalers>("Atmos::Spatial::Scalers2D", "previousScalers")
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}