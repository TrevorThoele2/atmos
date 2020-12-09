#pragma once

#include "AngelScriptName.h"

namespace Atmos::Scripting::Angel
{
    struct Namespaces
    {
        static inline const String name = "";

        struct Arca
        {
            static inline const String name = "Arca";
        };

        struct Atmos
        {
            static inline const String name = "Atmos";

            struct Asset
            {
                static inline const String name = CreateName({ Atmos::name }, "Asset");
            };

            struct Logging
            {
                static inline const String name = CreateName({ Atmos::name }, "Logging");
            };

            struct Spatial
            {
                static inline const String name = CreateName({ Atmos::name }, "Spatial");

                struct Grid
                {
                    static inline const String name = CreateName({ Spatial::name }, "Grid");
                };
            };

            struct Render
            {
                static inline const String name = CreateName({ Atmos::name }, "Render");
            };

            struct Time
            {
                static inline const String name = CreateName({ Atmos::name }, "Time");
            };

            struct Entity
            {
                static inline const String name = CreateName({ Atmos::name }, "Entity");
            };

            struct Scripting
            {
                static inline const String name = CreateName({ Atmos::name }, "Scripting");
            };
        };
    };
}