#pragma once

#include <vector>
#include <functional>
#include "Glyph.h"
#include "String.h"

namespace Atmos::Render
{
	struct TextLines
	{
		std::vector<std::vector<Glyph>> glyphs;
		float width;
	};

	[[nodiscard]] TextLines ToLines(
		const std::u32string& string, float wrapWidth, const std::function<Glyph(char32_t)>& retrieveGlyph);
}