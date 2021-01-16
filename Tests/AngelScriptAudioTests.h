#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/AudioAsset.h>

class AngelScriptAudioTestsFixture : public AngelScriptFixture
{
public:
    Arca::Index<Atmos::Asset::Audio> CreateAudioAsset(Arca::Reliquary& reliquary);
};