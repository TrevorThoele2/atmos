#pragma once

#include "FieldComponent.h"
#include "Script.h"

namespace Atmos
{
    // Holds scripts which are not held by other objects
    // Mainly used with actions/scripts to run other scripts
    class OrphanScripts : public FieldComponent
    {
    private:
        typedef std::list<Script::Instance> Held;
        Held held;
    public:
        OrphanScripts(Field &field);
        OrphanScripts(Field &field, OrphanScripts &&arg);
        OrphanScripts& operator=(OrphanScripts &&arg);

        Script::Instance& Add(Script::Instance &&script);

        void Work();
    };
}