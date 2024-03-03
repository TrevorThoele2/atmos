#pragma once

#include "String.h"
#include "FilePath.h"

namespace Atmos::Process
{
	struct Output
	{
		String standard;
		String error;
	};

	Output Run(const String& path);
}