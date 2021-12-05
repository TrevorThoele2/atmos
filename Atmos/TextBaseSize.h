#pragma once

#include <Arca/Signal.h>
#include "String.h"
#include "Size2D.h"
#include "FontAsset.h"

namespace Atmos::Render
{
	struct TextBaseSize
	{
		String string;
		Arca::RelicID font;
		float wrapWidth;
		bool bold;
		bool italics;
	};
}

namespace Arca
{
	template<>
	struct Traits<Atmos::Render::TextBaseSize>
	{
		static constexpr ObjectType objectType = ObjectType::Command;
		static const inline TypeName typeName = "Atmos::Render::TextBaseSize";
		using Result = Atmos::Spatial::Size2D;
	};
}