#pragma once

#include <vector>
#include "ScriptModule.h"
#include "ScriptCompiledModule.h"
#include "Buffer.h"
#include "Logger.h"

namespace Atmos::Scripting::JavaScript
{
	std::vector<Scripting::CompiledModule> Compile(
		const std::vector<Scripting::Module>& modules, Logging::Logger& logger);
}