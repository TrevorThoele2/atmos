#pragma once

#include <vector>

class asIScriptFunction;
class asIScriptContext;
class asIScriptEngine;

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Script::Angel
{
    class ScriptCurator;

    class Event
    {
    public:
        Event(Arca::Reliquary& reliquary);
        ~Event();

        void Subscribe(asIScriptFunction* function);
        void Unsubscribe(asIScriptFunction* function);

        void Execute();

        static void RegisterTo(asIScriptEngine* engine);
    private:
        asIScriptContext* context;
        std::vector<asIScriptFunction*> subscribedFunctions;

        asIScriptEngine* Engine();
    private:
        Arca::Reliquary* reliquary;
        ScriptCurator* FindSystem();
    };
}