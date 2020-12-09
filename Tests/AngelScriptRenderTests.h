#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/ImageAsset.h>

class AngelScriptRenderTestsFixture : public AngelScriptFixture
{
public:
    Arca::Index<Atmos::Asset::Image> CreateImageAsset(Arca::Reliquary& reliquary);
};