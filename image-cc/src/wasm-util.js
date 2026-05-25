import createModule from "./test.js";

let Module;
let api;

export async function initWasm() {
    Module = await createModule();

    api = {
        start: Module.cwrap('start'),
        end: Module.cwrap('end'),
        createNode: Module.cwrap('createNode', null, ['number', 'number']),
        connectNodes: Module.cwrap(
            'connectNodes',
            null,
            ['number', 'number', 'number', 'number']
        )
    };

    return api;
}