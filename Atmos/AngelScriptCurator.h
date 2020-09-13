#pragma once

#include <memory>

#include <Arca/Curator.h>

#include "Serialization.h"

class asIScriptEngine;

namespace Atmos::Script::Angel
{
    class ScriptCurator final : public Arca::Curator
    {
    public:
        explicit ScriptCurator(Init init);

        asIScriptEngine* Engine();
    private:
        class Interface;
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
        static inline const TypeName typeName = "Atmos::Script::Angel::ScriptCurator";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Script::Angel::ScriptCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Script::Angel::ScriptCurator>;
    };
}