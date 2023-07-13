import { Atmos } from "./atmos";

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
};