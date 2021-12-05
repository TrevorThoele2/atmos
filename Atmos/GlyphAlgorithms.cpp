#include "GlyphAlgorithms.h"

namespace Atmos::Render
{
    TextLines ToLines(
        const std::u32string& string, float wrapWidth, const std::function<Glyph(char32_t)>& retrieveGlyph)
    {
        float maxLineLength = 0;
        std::vector<std::vector<Glyph>> lines;
        std::vector<Glyph> line;
        std::vector<Glyph> chunk;
        float lineLength = 0;
        float chunkLength = 0;
        bool isChunkSpace = false;

        const auto appendChunkToLine = [&]()
        {
            line.insert(line.end(), chunk.begin(), chunk.end());
            chunk.clear();
            lineLength += chunkLength;
            chunkLength = 0;
        };

        const auto appendLineToLines = [&]()
        {
            lines.push_back(line);
            line.clear();
            maxLineLength = std::max(maxLineLength, lineLength);
            lineLength = 0;
        };

        const auto appendGlyphToChunk = [&](char32_t codepoint)
        {
            auto glyph = retrieveGlyph(codepoint);
            chunk.push_back(glyph);
            chunkLength += glyph.advance;
        };

        const auto discardChunk = [&]()
        {
            chunk.clear();
            chunkLength = 0;
        };

        const auto isChunkPastWrapWidth = [&]()
        {
            return lineLength + chunkLength >= wrapWidth;
        };

        for (auto& codepoint : string)
        {
            if (codepoint == U'\n')
            {
                if (chunkLength > 0)
                {
                    if (!isChunkSpace)
                        appendChunkToLine();
                    else
                        discardChunk();
                }

                appendLineToLines();
            }
            else if (codepoint == U' ')
            {
                if (!isChunkSpace)
                {
                    isChunkSpace = true;
                    appendChunkToLine();
                }

                appendGlyphToChunk(U' ');
            }
            else
            {
                if (isChunkSpace)
                {
                    if (!isChunkPastWrapWidth())
                        appendChunkToLine();
                    else
                        discardChunk();
                    isChunkSpace = false;
                    appendGlyphToChunk(codepoint);
                }
                else
                {
                    appendGlyphToChunk(codepoint);
                    if (isChunkPastWrapWidth() && lineLength != 0)
                        appendLineToLines();
                }
            }
        }

        if (!isChunkSpace && chunkLength > 0)
        {
            appendChunkToLine();
            appendLineToLines();
        }

        return { lines, maxLineLength };
    }
}