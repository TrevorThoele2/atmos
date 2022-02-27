import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send(Atmos.Traits.Audio.CreatePositionedSound.typeName, {"asset":{"id":23,"name":"aIyCtRueho"},"volume":1.10825e+38,"position":{"x":2.18804e+38,"y":2.30124e+38,"z":2.03409e+37}})
    const found = Atmos.Reliquary.find(Atmos.Traits.Audio.PositionedSound.typeName, created.id);
    Atmos.Reliquary.send(Atmos.Traits.Audio.DestroyPositionedSound.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};