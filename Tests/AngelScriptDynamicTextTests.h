#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/FontAsset.h>

class AngelScriptDynamicTextTestsFixture : public AngelScriptFixture
{
public:
    Arca::Index<Atmos::Asset::Font> CreateFontAsset(Arca::Reliquary& reliquary);
};