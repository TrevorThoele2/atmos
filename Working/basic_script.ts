import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send(Atmos.Traits.Spatial.CreateBounds.typeName, {"id":22,"space":0,"position":{"x":-8.87186,"y":92.2766,"z":44.7672},"baseSize":{"width":-95.7621,"height":-99.7697},"scalers":{"x":-23.1067,"y":-83.1081},"rotation":3.14395e+38})
    const found = Atmos.Reliquary.find(Atmos.Traits.Spatial.Bounds.typeName, created.id);
    Atmos.Reliquary.send(Atmos.Traits.Spatial.DestroyBounds.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};