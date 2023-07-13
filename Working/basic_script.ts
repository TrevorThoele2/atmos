import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send(Atmos.Traits.Spatial.CreateBounds.typeName, {"id":23,"space":0,"position":{"x":-87.426,"y":-72.3045,"z":91.0275},"baseSize":{"width":-67.0055,"height":37.1286},"scalers":{"x":70.2952,"y":-14.8476},"rotation":2.33382e+38})
    const found = Atmos.Reliquary.find(Atmos.Traits.Spatial.Bounds.typeName, created.id);
    Atmos.Reliquary.send(Atmos.Traits.Spatial.DestroyBounds.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};