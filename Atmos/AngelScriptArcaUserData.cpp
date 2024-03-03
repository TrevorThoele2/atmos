#include "AngelScriptArcaUserData.h"

#include <angelscript.h>
#include "AngelScriptResultVerification.h"

namespace Atmos::Scripting::Angel
{
    ArcaUserData* ArcaUserData::RequiredFrom(asIScriptEngine& engine)
    {
        const auto userData = reinterpret_cast<ArcaUserData*>(engine.GetUserData());
        if (!userData)
            throw Error("Could not find user data from script engine.");

        return userData;
    }

    ArcaUserData* ArcaUserData::RequiredFrom(asIScriptGeneric& generic)
    {
        const auto engine = generic.GetEngine();
        return RequiredFrom(*engine);
    }

    std::vector<asIScriptFunction*>* ArcaUserData::RequiredFunctionsFrom(asIScriptGeneric& generic)
    {
        const auto userData = RequiredFrom(generic);
        auto& functions = userData->functions;
        return &functions;
    }
}