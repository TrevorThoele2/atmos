#pragma once

#include "GeneralFixture.h"

#include "JavaScriptAutomation.h"

class RenderingFixture : public GeneralFixture
{
public:
    RenderingFixture();

    Arca::Index<Asset::Script> CompileAndCreateBasicMaterialScript(Arca::Reliquary& reliquary);
    [[nodiscard]] Buffer BasicMaterialScriptBytes(Arca::Reliquary& reliquary);
private:
    [[nodiscard]] static String BasicMaterialScriptSource();
};