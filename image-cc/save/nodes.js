const nodes = {
    input: makeNode(
        "input",
        0,
        1,
        "base",
        {},
        `
        <div>
            <div class="title-box">
                <i class="base"></i>
                Input Image
            </div>
        </div>
        `
    ),
    output: makeNode(
        "output",
        1,
        0,
        "base",
        {},
        `
        <div>
            <div class="title-box">
                <i class="base"></i>
                Output Image
            </div>
        </div>
        `
    ),
};

function makeNode(
    name,
    inputs,
    outputs,
    className,
    data,
    html
) {
    return {
        name,
        inputs,
        outputs,
        className,
        data,
        html
    };
}


export function addNodeToDrawFlow(name, pos_x, pos_y) {
    if (editor.editor_mode === "fixed") {
        return false;
    }

    pos_x =
        pos_x *
        (editor.precanvas.clientWidth /
        (editor.precanvas.clientWidth * editor.zoom)) -

        (editor.precanvas.getBoundingClientRect().x *
        (editor.precanvas.clientWidth /
        (editor.precanvas.clientWidth * editor.zoom)));

    pos_y =
        pos_y *
        (editor.precanvas.clientHeight /
        (editor.precanvas.clientHeight * editor.zoom)) -

        (editor.precanvas.getBoundingClientRect().y *
        (editor.precanvas.clientHeight /
        (editor.precanvas.clientHeight * editor.zoom)));

    const node = nodes[name];

    if (!node) {
        console.warn("Unknown node:", name);
        return;
    }

    editor.addNode(
        name,
        node.inputs,
        node.outputs,
        pos_x,
        pos_y,
        node.className,
        node.data,
        node.html
    );
}