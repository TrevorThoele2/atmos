#include "AngelScriptUserData.h"

#include <angelscript.h>
#include "AngelScriptResultVerification.h"

namespace Atmos::Scripting::Angel
{
    UserData* UserData::RequiredFrom(asIScriptEngine& engine)
    {
        const auto userData = static_cast<UserData*>(engine.GetUserData());
        if (!userData)
            throw Error("Could not find user data from script engine.");

        return userData;
    }

    UserData* UserData::RequiredFrom(asIScriptGeneric& generic)
    {
        const auto engine = generic.GetEngine();
        return RequiredFrom(*engine);
    }

    Arca::Reliquary* UserData::RequiredReliquaryFrom(asIScriptGeneric& generic)
    {
        const auto userData = RequiredFrom(generic);
        const auto reliquary = userData->reliquary;
        if (!reliquary)
            throw Error("Could not find reliquary from script engine.");

        return reliquary;
    }

    std::vector<UserData::SignalToRegister>* UserData::RequiredSignalsToRegister(asIScriptEngine& engine)
    {
        const auto userData = RequiredFrom(engine);
        return &userData->signalsToRegister;
    }
}