#include "AngelScriptUserData.h"

#include <angelscript.h>
#include "AngelScriptResultVerification.h"

namespace Atmos::Scripting::Angel
{
    UserData* UserData::RequiredFrom(asIScriptEngine& engine)
    {
        const auto userData = reinterpret_cast<UserData*>(engine.GetUserData());
        if (!userData)
            throw AngelScriptFailed("Could not find user data from script engine.");

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
            throw AngelScriptFailed("Could not find reliquary from script engine.");

        return reliquary;
    }
}