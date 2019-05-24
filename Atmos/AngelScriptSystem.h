#pragma once

#include <memory>
#include "ObjectSystem.h"

class asIScriptEngine;

namespace Atmos
{
    namespace Scripting
    {
        class System : public ObjectSystem
        {
        private:
            class Interface;
        public:
            System(ObjectManager& manager);

            asIScriptEngine* Engine();
        private:
            typedef std::unique_ptr<Interface> InterfacePtr;
            InterfacePtr interface;
        };
    }
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::Scripting::System> : public InscripterBase<::Atmos::Scripting::System>
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
    };
}