#pragma once

#include <vector>

class asIScriptFunction;
class asIScriptContext;
class asIScriptEngine;

namespace Atmos
{
    class ObjectManager;

    namespace Scripting
    {
        class System;

        class Event
        {
        public:
            Event(ObjectManager& objectManager);
            ~Event();

            void Subscribe(asIScriptFunction* function);
            void Unsubscribe(asIScriptFunction* function);

            void Execute();

            static void RegisterToAngelScript(asIScriptEngine* engine);
        private:
            asIScriptContext* context;
            std::vector<asIScriptFunction*> subscribedFunctions;

            asIScriptEngine* Engine();
        private:
            ObjectManager* objectManager;
            System* FindSystem();
        };
    }
}