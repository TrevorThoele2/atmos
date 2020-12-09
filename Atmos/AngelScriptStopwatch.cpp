#include "AngelScriptStopwatch.h"

#include "CreateStopwatch.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptGlobalManagement.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Time::Stopwatch>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Method(&Management::Method<&Start>, "int64", "Start", {})
            .Method(&Management::Method<&Calculate>, "int64", "Calculate", {})
            .Method(&Management::Method<&Type::ResetAverage>, "void", "ResetAverage", {})
            .Method(&Management::Method<&Type::ResetHighest>, "void", "ResetHighest", {})
            .ConstMethod(&Management::Method<&Elapsed>, "int64", "Elapsed", {})
            .ConstMethod(&Management::Method<&Type::IsStarted>, "bool", "IsStarted", {})
            .ConstMethod(&Management::Method<&Type::CurrentTime>, "int64", "CurrentTime", {})
            .ConstMethod(&Management::Method<&Type::Average>, "int64", "Average", {})
            .ConstMethod(&Management::Method<&Type::Highest>, "int64", "Highest", {})
            .Actualize(engine);

        GlobalRegistration(containingNamespace)
            .Function(
                &GlobalManagement::Function<&Time::CreateRealStopwatch>,
                "Atmos::Time::Stopwatch",
                "CreateRealStopwatch", {})
            .Function(
                &GlobalManagement::Function<&CreateFrameStopwatch, &UserData::RequiredReliquaryFrom>,
                "Atmos::Time::Stopwatch",
                "CreateFrameStopwatch",
                {})
            .Actualize(engine);
    }

    long long Registration<Time::Stopwatch>::Start(Type type)
    {
        const auto fromMethod = std::chrono::time_point_cast<Time::Seconds>(type.Start());
        return fromMethod.time_since_epoch().count();
    }

    long long Registration<Time::Stopwatch>::Calculate(Type type)
    {
        const auto fromMethod = std::chrono::duration_cast<Time::Seconds>(type.Calculate());
        return fromMethod.count();
    }

    long long Registration<Time::Stopwatch>::Elapsed(Type type)
    {
        const auto fromMethod = std::chrono::duration_cast<Time::Seconds>(type.Elapsed());
        return fromMethod.count();
    }

    long long Registration<Time::Stopwatch>::CurrentTime(Type type)
    {
        const auto fromMethod = std::chrono::time_point_cast<Time::Seconds>(type.CurrentTime());
        return fromMethod.time_since_epoch().count();
    }

    long long Registration<Time::Stopwatch>::Average(Type type)
    {
        const auto fromMethod = std::chrono::duration_cast<Time::Seconds>(type.Average());
        return fromMethod.count();
    }

    long long Registration<Time::Stopwatch>::Highest(Type type)
    {
        const auto fromMethod = std::chrono::duration_cast<Time::Seconds>(type.Highest());
        return fromMethod.count();
    }

    Time::Stopwatch Registration<Time::Stopwatch>::CreateFrameStopwatch(Arca::Reliquary* reliquary)
    {
        const auto frameInformation = Arca::Index<Frame::Information>(reliquary);
        return Time::CreateFrameStopwatch(*frameInformation);
    }
}