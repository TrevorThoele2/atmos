#pragma once

#include <memory>

#include "ObjectSystem.h"

#include "Serialization.h"

class asIScriptEngine;

namespace Atmos::Script
{
    class ScriptSystem : public ObjectSystem
    {
    private:
        class Interface;
    public:
        ScriptSystem(ObjectManager& manager);

        asIScriptEngine* Engine();
    private:
        typedef std::unique_ptr<Interface> InterfacePtr;
        InterfacePtr interface;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::ScriptSystem, BinaryArchive> :
        public CompositeScribe<::Atmos::Script::ScriptSystem, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };
}