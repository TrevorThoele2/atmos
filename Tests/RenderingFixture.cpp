#include "RenderingFixture.h"

#include <Atmos/CompileScript.h>

RenderingFixture::RenderingFixture()
{}

Arca::Index<Asset::Script> RenderingFixture::CompileAndCreateBasicMaterialScript(
    Arca::Reliquary& reliquary)
{
    return CompileAndCreateScriptAsset("basic_script", BasicMaterialScriptSource(), reliquary);
}

Buffer RenderingFixture::BasicMaterialScriptBytes(Arca::Reliquary& reliquary)
{
    const auto compiledModules = reliquary.Do(Scripting::Compile{{ Scripting::Module{ "basic_script", BasicMaterialScriptSource() }} });

    std::vector<Scripting::CompiledModule>::const_iterator compiledSource;
    for (const auto& compiledModule : compiledModules)
        if (compiledModule.name == Inscription::File::Path("basic_script.ts").replace_extension().string())
            return compiledModule.source;

    throw std::runtime_error("Could not find module.");
}

String RenderingFixture::BasicMaterialScriptSource()
{
    return R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const material = Atmos.Reliquary.find(Atmos.Traits.Render.Raster.ExecutingMaterial.typeName)!;
    
    const shaders = {
        vertex: Atmos.Reliquary.send(Atmos.Traits.Asset.FindByName.Shader.typeName, { name: "vertex" }),
        fragment: Atmos.Reliquary.send(Atmos.Traits.Asset.FindByName.Shader.typeName, { name: "fragment" })    
    };

    Atmos.Reliquary.send(
        Atmos.Traits.Render.Raster.RecordCommands.typeName,
        {
            commands: [
                ...material.images.map(x => ({ shaders, ...x })),
                ...material.lines.map(x => ({ shaders, ...x })),
                ...material.regions.map(x => ({ shaders, ...x })),
                ...material.texts.map(x => ({ shaders, ...x }))
            ]
        });

    return {
        done: true
    };
};)V0G0N";
}