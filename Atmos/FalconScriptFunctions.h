#pragma once

#include "FalconScriptFunctionPrototype.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Functions
        {
            namespace General
            {
                extern Prototype Suspend;
                extern Prototype LogMessage;
                extern Prototype RandomBool;
                extern Prototype RandomInteger;
            }

            namespace Position
            {
                extern Prototype FindPath;
            }

            namespace Input
            {
                extern Prototype IsActionActive;
                extern Prototype IsActionPressed;
                extern Prototype IsActionDepressed;
            }

            namespace Modulator
            {
                extern Prototype Create;
            }

            namespace Speech
            {
                extern Prototype SetCharacters;
                extern Prototype AppendCharacters;
                extern Prototype ClearCharacters;
                extern Prototype ActivateInput;
                extern Prototype DeactivateInput;
                extern Prototype GetInputPosition;
                extern Prototype Leave;
            }

            namespace Shop
            {
                extern Prototype Enter;
                extern Prototype Leave;
                extern Prototype IsActive;
            }

            namespace Party
            {
                extern Prototype Add;
                extern Prototype IsEntityIn;
            }

            namespace Entity
            {
                extern Prototype GetAvatar;
            }
        }
    }
}