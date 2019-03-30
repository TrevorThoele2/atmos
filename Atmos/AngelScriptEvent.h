#pragma once

#include <vector>

class asIScriptFunction;
class asIScriptContext;
class asIScriptEngine;

namespace Atmos
{
    namespace Scripting
    {
        class Event
        {
        public:
            Event();
            ~Event();

            static void Constructor(void* memory);
            static void Destructor(void* memory);

            void Subscribe(asIScriptFunction* function);
            void Unsubscribe(asIScriptFunction* function);

            void Execute();

            static void RegisterToAngelScript(asIScriptEngine* engine);
        private:
            asIScriptContext* context;
            std::vector<asIScriptFunction*> subscribedFunctions;
        private:
            static asIScriptEngine* Engine();
        };
    }
}