import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send(Atmos.Traits.Spatial.CreateBounds.typeName, {"id":23,"space":0,"position":{"x":74.4894,"y":-76.5493,"z":-1.10086},"baseSize":{"width":-26.1943,"height":-69.4132},"scalers":{"x":-23.7393,"y":94.5714},"rotation":2.3427e+38})
    const found = Atmos.Reliquary.find(Atmos.Traits.Spatial.Bounds.typeName, created.id);
    Atmos.Reliquary.send(Atmos.Traits.Spatial.DestroyBounds.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};