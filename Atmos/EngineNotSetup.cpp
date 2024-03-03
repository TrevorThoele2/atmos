#include "EngineNotSetup.h"

namespace Atmos
{
    EngineNotSetup::EngineNotSetup() :
        runtime_error("The engine has not been setup and so does not support this operation.")
    {}
}