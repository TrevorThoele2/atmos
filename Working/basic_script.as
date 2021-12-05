import * as atmos from "atmos";

export const main = (loggedMessage: string): Result => {
    atmos.log({ message: loggedMessage });
    return {
        done: true
    };
};