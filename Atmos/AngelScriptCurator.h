#pragma once

#include <memory>

#include <Arca/Curator.h>

#include "Serialization.h"

class asIScriptEngine;

namespace Atmos::Script::Angel
{
    class ScriptCurator final : public Arca::Curator
    {
    private:
        class Interface;
    public:
        ScriptCurator() = default;

        asIScriptEngine* Engine();
    protected:
        void InitializeImplementation() override;
    private:
        using InterfacePtr = std::unique_ptr<Interface>;
        InterfacePtr interface;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::Angel::ScriptCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "AngelScriptCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::Angel::ScriptCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Script::Angel::ScriptCurator, BinaryArchive>
    {};
}