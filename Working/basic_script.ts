import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send(Atmos.Traits.Spatial.CreateBounds.typeName, {"id":23,"space":0,"position":{"x":0.591103,"y":-99.3077,"z":-82.5803},"baseSize":{"width":-99.8061,"height":-58.4311},"scalers":{"x":-37.2834,"y":81.2087},"rotation":3.81189e+36})
    const found = Atmos.Reliquary.find(Atmos.Traits.Spatial.Bounds.typeName, created.id);
    Atmos.Reliquary.send(Atmos.Traits.Spatial.DestroyBounds.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};