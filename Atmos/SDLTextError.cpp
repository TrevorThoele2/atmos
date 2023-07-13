#include "SDLTextError.h"

#include "GraphicsError.h"
#include "SDLIncludes.h"

namespace Atmos::Render::SDL
{
	void VerifyResult(int result, const String& errorMessage)
	{
		if (result < 0)
			throw GraphicsError(errorMessage, { { "Reason", TTF_GetError() } });
	}
}