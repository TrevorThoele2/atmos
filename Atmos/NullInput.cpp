
#include "NullInput.h"

namespace Atmos
{
    namespace Input
    {
        void NullHandler::AddInputsImpl()
        {}

        void NullHandler::WorkInputsImpl(InputVector &inputs)
        {}

        bool NullHandler::ShouldAddActions() const
        {
            return false;
        }

        MousePosition NullHandler::GetMousePositionImpl() const
        {
            return MousePosition();
        }
    }
}