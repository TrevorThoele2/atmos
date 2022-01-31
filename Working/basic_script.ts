import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send(Atmos.Traits.Spatial.CreateBounds.typeName, {"id":22,"space":0,"position":{"x":67.8404,"y":-81.0488,"z":54.4113},"baseSize":{"width":51.5147,"height":-80.6528},"scalers":{"x":-24.0422,"y":11.0352},"rotation":2.16605e+38})
    const found = Atmos.Reliquary.find(Atmos.Traits.Spatial.Bounds.typeName, created.id);
    Atmos.Reliquary.send(Atmos.Traits.Spatial.DestroyBounds.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};