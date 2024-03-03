#pragma once

#include "ObjectSystem.h"

class asIScriptEngine;

namespace Atmos
{
    namespace Scripting
    {
        class EngineManager : public ObjectSystem
        {
        public:
            EngineManager(ObjectManager& manager);

            asIScriptEngine* Engine();
        private:
            asIScriptEngine* engine;
        };
    }
}