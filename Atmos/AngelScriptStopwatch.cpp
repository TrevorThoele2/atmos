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
            .Method(&Management::Method<&Type::Restart>, "Atmos::Time::Point<Atmos::Time::Nanoseconds>", "Restart", {})
            .Method(&Management::Method<&Type::Calculate>, "Atmos::Time::Nanoseconds", "Calculate", {})
            .Method(&Management::Method<&Type::ResetAverage>, "void", "ResetAverage", {})
            .Method(&Management::Method<&Type::ResetHighest>, "void", "ResetHighest", {})
            .ConstMethod(&Management::Method<&Type::Elapsed>, "Atmos::Time::Nanoseconds", "Elapsed", {})
            .ConstMethod(&Management::Method<&Type::CurrentTime>, "Atmos::Time::Point<Atmos::Time::Nanoseconds>", "CurrentTime", {})
            .ConstMethod(&Management::Method<&Type::Average>, "Atmos::Time::Nanoseconds", "Average", {})
            .ConstMethod(&Management::Method<&Type::Highest>, "Atmos::Time::Nanoseconds", "Highest", {})
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

    Time::Stopwatch Registration<Time::Stopwatch>::CreateFrameStopwatch(Arca::Reliquary* reliquary)
    {
        const auto frameInformation = Arca::Index<Frame::Information>(reliquary);
        return Time::CreateFrameStopwatch(*frameInformation);
    }
}