#include "AngelScriptRegistrationInterface.h"

namespace Atmos::Script::Angel
{
    UserData* RegistrationInterface::UserDataFrom(asIScriptGeneric* gen)
    {
        auto engine = gen->GetEngine();
        return reinterpret_cast<UserData*>(engine->GetUserData());
    }
}