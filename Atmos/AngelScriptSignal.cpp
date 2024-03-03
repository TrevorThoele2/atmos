#include "AngelScriptSignal.h"

#include "CurrentExecutingScript.h"

namespace Atmos::Scripting::Angel
{
    namespace Detail
    {
        Arca::Index<Script> RetrieveCurrentExecuting(Arca::Reliquary& reliquary)
        {
            const auto currentRunning = Arca::Index<CurrentExecutingScript>(reliquary);
            if (currentRunning)
                return Arca::Index<Script>(currentRunning->id, reliquary);
            else
                return {};
        }
    }
}