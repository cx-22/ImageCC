const telegramData = {
    name: 0,
    inputs: 1,
    outputs: 1,
    pos_x: 0,
    pos_y: 0,
    className: "telegram",
    data: {
        channel: "channel_3"
    },
    html: `
            <div>
                <div class="title-box"><i class="fab fa-telegram-plane"></i> Telegram bot</div>
                <div class="box">
                    <p>Send to telegram</p>
                    <p>select channel</p>
                    <select df-channel>
                    <option value="channel_1">Channel 1</option>
                    <option value="channel_2">Channel 2</option>
                    <option value="channel_3">Channel 3</option>
                    <option value="channel_4">Channel 4</option>
                    </select>
                </div>
            </div>
        `
};

const imageInputNode = {
    name: 0,
    inputs: 0,
    outputs: 1,
    pos_x: 0,
    pos_y: 0,
    className: "",
    data: {
        image: null
    },
    html: `
        <div>
            <div class="title-box">
                <i class="fas fa-image"></i> Image Input
            </div>

            <div class="box">

                <!-- Drag & Drop Zone -->
                <div class="image-drop-zone" style="
                    border: 2px dashed #888;
                    padding: 10px;
                    text-align: center;
                    cursor: pointer;
                    margin-bottom: 8px;
                ">
                    Drop image here
                </div>

                <!-- File Button -->
                <button class="image-select-btn">
                    Select Image
                </button>

                <!-- Hidden file input -->
                <input type="file" accept="image/*" class="image-file-input" style="display:none;" />

            </div>
        </div>
    `
};

const imageOutputNode = {
    name: 0,
    inputs: 1,
    outputs: 0,
    pos_x: 0,
    pos_y: 0,
    className: "",
    data: {
        image: null
    },
    html: `
        <div>
            <div class="title-box">
                <i class="fas fa-image"></i> Image Input
            </div>

            <div class="box">
                Output Image
            </div>
        </div>
    `
};

const additionNode = {
    name: 0,
    inputs: 1,
    outputs: 1,
    pos_x: 0,
    pos_y: 0,
    className: "",
    data: {
        value1: 50
    },
    html: `
            <div>
                <div class="title-box"><i class="fab fa-telegram-plane"></i> Telegram bot</div>
                <div class="box">
                    <p>add 2 numbers</p>
                    <div class="input-feilds">
                        <input type="number" min="0" max="255" step="1" value="50">
                    </div>
                </div>
            </div>
        `
};

export const node_map = new Map([
    ["0", imageInputNode],
    ["1", imageOutputNode],
    ["2", additionNode]
]);

function bindNodeInputs(editor, nodeId, nodeData) {

    // drawflow node element
    const nodeElement =
        document.querySelector(`#node-${nodeId}`);

    if (!nodeElement) return;

    const fieldContainer =
        nodeElement.querySelector(".input-feilds");

    if (!fieldContainer) return;

    const inputs =
        fieldContainer.querySelectorAll("input");

    const keys =
        Object.keys(nodeData);

    inputs.forEach((input, index) => {

        const key = keys[index];

        if (!key) return;

        // initialize input from data
        input.value = nodeData[key];

        // update data on change
        input.addEventListener("input", (e) => {

            let value = e.target.value;

            // optional automatic numeric conversion
            if (!isNaN(value) && value.trim() !== "") {
                value = Number(value);
            }

            nodeData[key] = value;

            document.dispatchEvent(
                new CustomEvent("nodeParamChange", {
                    detail: {
                        nodeId: nodeId,
                        value: value,
                        argumentIndex: index
                    }
                })
            );
        });
    });
}

export function addNodeWrapper(editor, args, pos_x, pos_y) {

    if (!args || !args.data) {
        return;
    }

    const nodeData = structuredClone(args.data);

    const nodeId = editor.addNode(
        args.name,
        args.inputs,
        args.outputs,
        pos_x,
        pos_y,
        args.className,
        nodeData,
        args.html
    );

    bindNodeInputs(editor, nodeId, nodeData);

    return nodeId;
}


/*
export function addNodeWrapper(editor, args, pos_x, pos_y) {
    console.log("args.data");
    console.log(args.data);
    if (!Array.isArray(args)) {
        return editor.addNode(
            args.name,
            args.inputs,
            args.outputs,
            pos_x,
            pos_y,
            args.className,
            args.data,
            args.html
        );
    }
}
*/

const items = [
        ["Input Image", "Sys"],
        ["Output Image", "Sys"],
        ["Addition", "Arithmetic"],
        ["Subtraction", "Arithmetic"],
    ];

const categoryIcons =     {
        //"Arithmetic": "../assets/icons/art.png"
        "Sys": "../assets/icons/art.png"
    };

export function buildCategoryDropdowns() {
    const cols = document.querySelectorAll(".col");
    if (!cols.length) return;

    const parent = cols[0];

    const categories = {};
    let nodeId = 0;

    for (const [item, category] of items) {
        if (!categories[category]) {
            categories[category] = [];
        }

        categories[category].push(item);
    }

    for (const category in categories) {
        const dropdown = document.createElement("div");
        dropdown.className = "dropdown";

        const button = document.createElement("button");
        button.className = "dropdown-button";

        const iconPath = categoryIcons[category];

        if (iconPath) {
            const img = document.createElement("img");
            img.src = iconPath;
            img.style.width = "32px";
            img.style.height = "32px";
            img.style.marginRight = "6px";
            button.appendChild(img);
        }

        const label = document.createElement("span");
        label.textContent = category;

        button.appendChild(label);

        const menu = document.createElement("div");
        menu.className = "dropdown-menu";

        for (const item of categories[category]) {
            const div = document.createElement("div");

            div.className = "drag-drawflow";
            div.draggable = true;

            div.dataset.node = nodeId++; // <-- incremental ID

            div.innerHTML = `<span>${item}</span>`;

            menu.appendChild(div);
        }

        button.addEventListener("click", () => {
            dropdown.classList.toggle("open");
        });

        dropdown.appendChild(button);
        dropdown.appendChild(menu);

        parent.appendChild(dropdown);
    }
}