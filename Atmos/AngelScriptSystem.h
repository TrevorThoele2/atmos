#pragma once

#include <memory>

#include "ObjectSystem.h"

#include "Serialization.h"

class asIScriptEngine;

namespace Atmos::Scripting
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

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Scripting::System, BinaryArchive> :
        public CompositeScribe<::Atmos::Scripting::System, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}