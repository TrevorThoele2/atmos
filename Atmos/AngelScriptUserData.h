#pragma once

namespace Atmos
{
    class ObjectManager;
}

namespace Atmos::Script
{
    class ScriptSystem;

    class UserData
    {
    public:
        ObjectManager* objectManager;
        ScriptSystem* system;
        UserData();
    };
}