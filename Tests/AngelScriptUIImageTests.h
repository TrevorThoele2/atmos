#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/ImageAsset.h>

class AngelScriptUIImageTestsFixture : public AngelScriptFixture
{
public:
    Arca::Index<Atmos::Asset::Image> CreateImageAsset(Arca::Reliquary& reliquary);
};