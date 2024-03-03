#pragma once

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Script::Angel
{
    class ScriptCurator;

    struct UserData
    {
        Arca::Reliquary* reliquary = nullptr;
        ScriptCurator* curator = nullptr;
    };
}