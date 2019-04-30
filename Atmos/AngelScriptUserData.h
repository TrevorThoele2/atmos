#pragma once

namespace Atmos
{
    class ObjectManager;

    namespace Scripting
    {
        class System;

        class UserData
        {
        public:
            ObjectManager* objectManager;
            System* system;
            UserData();
        };
    }
}