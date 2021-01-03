#pragma once

#include "TimePoint.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptGlobalManagement.h"
#include "AngelScriptRegistration.h"
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptGlobalRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class GenericTimePoint
    {};

    template<>
    struct Registration<GenericTimePoint>
    {
        using Type = GenericTimePoint;

        static inline const String name = "Point<class T>";
        static inline const String containingNamespace = "Atmos::Time";
        static inline const String documentation = "Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class Units>
    struct TimePointCast
    {};

    template<class Units>
    struct Registration<TimePointCast<Units>>
    {
        using Type = TimePointCast<Units>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "TimePointCast";
        static inline const String containingNamespace = "Atmos::Time";
        static inline const String documentation = "";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        template<class To, class From>
        static Time::Point<To> PointCast(From from);
    };

    template<class Units>
    void Registration<TimePointCast<Units>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using TimePoint = Time::Point<Units>;
        using RegistrationT = Registration<TimePoint>;
        const auto argumentTypeName = CreateName({ RegistrationT::containingNamespace }, RegistrationT::name);

        GlobalRegistration("Atmos::Time")
            .Function(
                &GlobalManagement::Function<&PointCast<Time::Hours, TimePoint>, &PullFromParameter<0, TimePoint>>,
                "Atmos::Time::Point<Atmos::Time::Hours>", "ToHours", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&PointCast<Time::Minutes, TimePoint>, &PullFromParameter<0, TimePoint>>,
                "Atmos::Time::Point<Atmos::Time::Minutes>", "ToMinutes", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&PointCast<Time::Seconds, TimePoint>, &PullFromParameter<0, TimePoint>>,
                "Atmos::Time::Point<Atmos::Time::Seconds>", "ToSeconds", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&PointCast<Time::Milliseconds, TimePoint>, &PullFromParameter<0, TimePoint>>,
                "Atmos::Time::Point<Atmos::Time::Milliseconds>", "ToMilliseconds", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&PointCast<Time::Microseconds, TimePoint>, &PullFromParameter<0, TimePoint>>,
                "Atmos::Time::Point<Atmos::Time::Microseconds>", "ToMicroseconds", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&PointCast<Time::Nanoseconds, TimePoint>, &PullFromParameter<0, TimePoint>>,
                "Atmos::Time::Point<Atmos::Time::Nanoseconds>", "ToNanoseconds", { argumentTypeName })
            .Actualize(engine, documentationManager);
    }

    template<class Units>
    template<class To, class From>
    Time::Point<To> Registration<TimePointCast<Units>>::PointCast(From from)
    {
        return std::chrono::time_point_cast<To>(from);
    }

    template<class Units>
    struct Registration<Time::Point<Units>>
    {
        using Type = Time::Point<Units>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Point<" + CreateName({ Registration<Units>::containingNamespace }, Registration<Units>::name) + ">";
        static inline const String containingNamespace = "Atmos::Time";
        static inline const String documentation = "Represents a point in time.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class Units>
    void Registration<Time::Point<Units>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using UnitsRegistration = Registration<Units>;
        const auto unitsName = CreateName({ UnitsRegistration::containingNamespace }, UnitsRegistration::name);

        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Constructor(&Management::template GenerateValue<&PullFromParameter<0, Units>>, { unitsName })
            .Compare(&Management::Compare)
            .Add(&Management::template Add<Time::Hours>, "Atmos::Time::Hours")
            .Add(&Management::template Add<Time::Minutes>, "Atmos::Time::Minutes")
            .Add(&Management::template Add<Time::Seconds>, "Atmos::Time::Seconds")
            .Add(&Management::template Add<Time::Milliseconds>, "Atmos::Time::Milliseconds")
            .Add(&Management::template Add<Time::Microseconds>, "Atmos::Time::Microseconds")
            .Add(&Management::template Add<Time::Nanoseconds>, "Atmos::Time::Nanoseconds")
            .Subtract(&Management::template Subtract<Time::Hours>, "Atmos::Time::Hours")
            .Subtract(&Management::template Subtract<Time::Minutes>, "Atmos::Time::Minutes")
            .Subtract(&Management::template Subtract<Time::Seconds>, "Atmos::Time::Seconds")
            .Subtract(&Management::template Subtract<Time::Milliseconds>, "Atmos::Time::Milliseconds")
            .Subtract(&Management::template Subtract<Time::Microseconds>, "Atmos::Time::Microseconds")
            .Subtract(&Management::template Subtract<Time::Nanoseconds>, "Atmos::Time::Nanoseconds")
            .CompoundAdd(&Management::template CompoundAdd<Units>, unitsName)
            .CompoundSubtract(&Management::template CompoundSubtract<Units>, unitsName)
            .ConstMethod(&Management::template Method<&Type::time_since_epoch>, unitsName, "TimeSinceEpoch", {})
            .Actualize(engine, documentationManager);
    }

    template<class Units>
    struct TimeDurationTraits;

    template<>
    struct TimeDurationTraits<Time::Hours>
    {
        static inline const String name = "Hours";
    };

    template<>
    struct TimeDurationTraits<Time::Minutes>
    {
        static inline const String name = "Minutes";
    };

    template<>
    struct TimeDurationTraits<Time::Seconds>
    {
        static inline const String name = "Seconds";
    };

    template<>
    struct TimeDurationTraits<Time::Milliseconds>
    {
        static inline const String name = "Milliseconds";
    };

    template<>
    struct TimeDurationTraits<Time::Microseconds>
    {
        static inline const String name = "Microseconds";
    };

    template<>
    struct TimeDurationTraits<Time::Nanoseconds>
    {
        static inline const String name = "Nanoseconds";
    };

    template<class Units>
    struct Registration<Time::Duration<Units>>
    {
        using Type = Time::Duration<Units>;
        using Management = ObjectManagement<Type>;

        static inline const String name = TimeDurationTraits<Type>::name;
        static inline const String containingNamespace = "Atmos::Time";
        static inline const String documentation = "Represents a duration of time.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class Units>
    void Registration<Time::Duration<Units>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Constructor(&Management::template GenerateValue<&PullFromParameter<0, long long>>, { "int64" })
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Compare(&Management::Compare)
            .Negation(&Management::Negation)
            .PrefixIncrement(&Management::PrefixIncrement)
            .PostfixIncrement(&Management::PostfixIncrement)
            .PrefixDecrement(&Management::PrefixDecrement)
            .PostfixDecrement(&Management::PostfixDecrement)
            .Add(&Management::Add)
            .Subtract(&Management::Subtract)
            .Multiply(&Management::template Multiply<long long>, "int64")
            .Divide(&Management::Divide)
            .Modulo(&Management::Modulo)
            .CompoundAdd(&Management::CompoundAdd)
            .CompoundSubtract(&Management::CompoundSubtract)
            .CompoundMultiply(&Management::template CompoundMultiply<long long>, "int64")
            .CompoundDivide(&Management::template CompoundDivide<long long>, "int64")
            .CompoundModulo(&Management::template CompoundModulo<long long>, "int64")
            .ConstMethod(&Management::template Method<&Type::count>, "int64", "Count", {})
            .Actualize(engine, documentationManager);
    }

    template<class Units>
    struct TimeDurationCast
    {};

    template<class Units>
    struct Registration<TimeDurationCast<Units>>
    {
        using Type = TimeDurationCast<Units>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "TimeDurationCast";
        static inline const String containingNamespace = "Atmos::Time";
        static inline const String documentation = "";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        template<class To, class From>
        static To DurationCast(From from);
    };

    template<class Units>
    void Registration<TimeDurationCast<Units>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using Duration = Units;
        using RegistrationT = Registration<Duration>;
        const auto argumentTypeName = CreateName({ RegistrationT::containingNamespace }, RegistrationT::name);

        GlobalRegistration("Atmos::Time")
            .Function(
                &GlobalManagement::Function<&DurationCast<Time::Hours, Duration>, &PullFromParameter<0, Duration>>,
                "Atmos::Time::Hours", "ToHours", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&DurationCast<Time::Minutes, Duration>, &PullFromParameter<0, Duration>>,
                "Atmos::Time::Minutes", "ToMinutes", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&DurationCast<Time::Seconds, Duration>, &PullFromParameter<0, Duration>>,
                "Atmos::Time::Seconds", "ToSeconds", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&DurationCast<Time::Milliseconds, Duration>, &PullFromParameter<0, Duration>>,
                "Atmos::Time::Milliseconds", "ToMilliseconds", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&DurationCast<Time::Microseconds, Duration>, &PullFromParameter<0, Duration>>,
                "Atmos::Time::Microseconds", "ToMicroseconds", { argumentTypeName })
            .Function(
                &GlobalManagement::Function<&DurationCast<Time::Nanoseconds, Duration>, &PullFromParameter<0, Duration>>,
                "Atmos::Time::Nanoseconds", "ToNanoseconds", { argumentTypeName })
            .Actualize(engine, documentationManager);
    }

    template<class Units>
    template<class To, class From>
    To Registration<TimeDurationCast<Units>>::DurationCast(From from)
    {
        return std::chrono::duration_cast<To>(from);
    }
}