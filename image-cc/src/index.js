import {node_map, addNodeWrapper, buildCategoryDropdowns} from "./nodes.js"
import {initWasm} from "./wasm-util.js"

let wasm;



const display = document.getElementById("display");

let img = new Image();
let imgLoaded = false;

let scale = 1;
let fitScale = 1;

let centerX = 0;
let centerY = 0;

let startX = 0;
let startY = 0;

let currentImageData = null;
let isPanning = false;

let needsRender = false;

//display
const canvas = document.createElement("canvas");
const ctx = canvas.getContext("2d");
display.appendChild(canvas);

function render() {

    if (!imgLoaded) return;

    const rect = display.getBoundingClientRect();
    const w = rect.width;
    const h = rect.height;

    canvas.width = w;
    canvas.height = h;

    // camera transform: top-left image origin
    const drawX = -centerX * scale;
    const drawY = -centerY * scale;

    ctx.setTransform(scale, 0, 0, scale, drawX, drawY);

    ctx.clearRect(0, 0, w, h);
    ctx.drawImage(img, 0, 0);
}

function requestRender() {
    if (needsRender) return;

    needsRender = true;

    requestAnimationFrame(() => {
        needsRender = false;
        render();
    });
}

function computeFitScale() {

    if (!imgLoaded) return;

    const w = display.clientWidth;
    const h = display.clientHeight;

    const scaleX = w / img.width;
    const scaleY = h / img.height;

    fitScale = Math.min(scaleX, scaleY);
}

// panning
let panStartMouseX = 0;
let panStartMouseY = 0;

display.addEventListener("mousedown", (e) => {

    isPanning = true;

    const rect = display.getBoundingClientRect();

    const mx = e.clientX - rect.left;
    const my = e.clientY - rect.top;

    startX = centerX;
    startY = centerY;

    panStartMouseX = mx;
    panStartMouseY = my;
});

window.addEventListener("mouseup", () => {
    isPanning = false;
});

window.addEventListener("mousemove", (e) => {

    if (!isPanning) return;

    const rect = display.getBoundingClientRect();

    const mx = e.clientX - rect.left;
    const my = e.clientY - rect.top;

    const dx = (mx - panStartMouseX) / scale;
    const dy = (my - panStartMouseY) / scale;

    centerX = startX - dx;
    centerY = startY - dy;

    requestRender();
});

// zooming (FIXED)
display.addEventListener("wheel", (e) => {

    if (!imgLoaded) return;

    e.preventDefault();

    const rect = display.getBoundingClientRect();

    const mx = e.clientX - rect.left;
    const my = e.clientY - rect.top;

    // STEP 1: convert mouse → image space (before zoom)
    const imgX = (mx / scale) + centerX;
    const imgY = (my / scale) + centerY;

    const zoomFactor = e.deltaY < 0 ? 1.1 : 1 / 1.1;

    const newScale = scale * zoomFactor;

    if (newScale < fitScale) {
        resetView();
        requestRender();
        return;
    }

    scale = newScale;

    // STEP 2: LOCK IMAGE POINT UNDER MOUSE
    // solve so imgX/imgY stays under cursor after scaling
    centerX = imgX - (mx / scale);
    centerY = imgY - (my / scale);

    requestRender();

}, { passive: false });

function loadImageFromPath(path) {

    img = new Image();

    img.onload = () => {

        imgLoaded = true;

        computeFitScale();

        scale = fitScale;

        const rect = display.getBoundingClientRect();
        const w = rect.width;
        const h = rect.height;

        centerX = (img.width - w / scale) / 2;
        centerY = (img.height - h / scale) / 2;

        // IMPORTANT FIX: ensure correct initial render size
        canvas.width = display.clientWidth;
        canvas.height = display.clientHeight;

        requestRender();
    };

    img.src = path;
}

function resetView() {

    computeFitScale();

    scale = fitScale;

    const rect = display.getBoundingClientRect();
    const w = rect.width;
    const h = rect.height;

    centerX = (img.width - w / scale) / 2;
    centerY = (img.height - h / scale) / 2;
}

function displayImage(image) {
    img = image;
    imgLoaded = true;
    currentImageData = image;

    resetView();
    requestRender();
}






// Splitter
const splitter = document.getElementById("splitter");
const bottom = document.getElementById("drawflow");

let dragging = false;

splitter.addEventListener("mousedown", () => {
    dragging = true;
});

window.addEventListener("mouseup", () => {
    dragging = false;
});

window.addEventListener("mousemove", (e) => {

    if (!dragging) return;

    const container = document.getElementById("container");
    const rect = container.getBoundingClientRect();

    const y = e.clientY - rect.top;

    document.getElementById("display").style.height = y + "px";
    bottom.style.height = (rect.height - y - 6) + "px";

    computeFitScale();

    // if not zoomed in, re-fit properly
    if (scale <= fitScale) {
        resetView();
    }

    requestRender();
});









function addNodeToDrawFlow(name, pos_x, pos_y) {
    if(editor.editor_mode === 'fixed') {
    return false;
    }
    pos_x = pos_x * ( editor.precanvas.clientWidth / (editor.precanvas.clientWidth * editor.zoom)) - (editor.precanvas.getBoundingClientRect().x * ( editor.precanvas.clientWidth / (editor.precanvas.clientWidth * editor.zoom)));
    pos_y = pos_y * ( editor.precanvas.clientHeight / (editor.precanvas.clientHeight * editor.zoom)) - (editor.precanvas.getBoundingClientRect().y * ( editor.precanvas.clientHeight / (editor.precanvas.clientHeight * editor.zoom)));


    addNodeWrapper(editor, node_map.get(name), pos_x, pos_y);
}

function allowDrop(ev) {
    ev.preventDefault();
}

function drag(ev) {
    if (ev.type === "touchstart") {
      mobile_item_selec = ev.currentTarget.closest(".drag-drawflow").getAttribute('data-node');
      console.log("check1");
    } else {
      ev.dataTransfer.setData("node", ev.currentTarget.getAttribute('data-node'));
      console.log("check2");
      console.log(ev.currentTarget.getAttribute('data-node'));
    }
}

function drop(ev) {
    if (ev.type === "touchend") {
    var parentdrawflow = document.elementFromPoint( mobile_last_move.touches[0].clientX, mobile_last_move.touches[0].clientY).closest("#drawflow");
    if(parentdrawflow != null) {
      console.log("xxxx")
        addNodeToDrawFlow(mobile_item_selec, mobile_last_move.touches[0].clientX, mobile_last_move.touches[0].clientY);
    }
    mobile_item_selec = '';
    } else {
    ev.preventDefault();
    var data = ev.dataTransfer.getData("node");
    console.log("yyyyyyy")
    addNodeToDrawFlow(data, ev.clientX, ev.clientY);
    }

}

function sendParam(nid, value, paramIndex) {
    let ptr;
    // int
    if (Number.isInteger(value)) {
        ptr = wasm.exports.malloc(4);
        new Int32Array(wasm.exports.memory.buffer)[ptr / 4] = value;
    }

    // float
    else if (typeof value === "number") {
        ptr = wasm.exports.malloc(4);
        new Float32Array(wasm.exports.memory.buffer)[ptr / 4] = value;
    }

    else if (typeof value === "string") {
        const bytes = new TextEncoder().encode(value + "\0");

        ptr = wasm.exports.malloc(bytes.length);
        new Uint8Array(
            wasm.exports.memory.buffer,
            ptr,
            bytes.length
        ).set(bytes);
    }

    wasm.addParam(
        nid,
        ptr,
        paramIndex  
    );
}




//new image ig
function attachImageNodeEvents(nodeEl, nodeData) {

    const dropZone = nodeEl.querySelector(".image-drop-zone");
    const fileInput = nodeEl.querySelector(".image-file-input");
    const button = nodeEl.querySelector(".image-select-btn");

    console.log("ok");

    function handleImage(file) {

        if (!file || !file.type.startsWith("image/")) {
            console.log("Invalid file");
            return;
        }

        const reader = new FileReader();

        reader.onload = (e) => {

            nodeData.data.image = e.target.result;

            console.log("Valid image loaded");

            // Example: trigger output update
            if (typeof nodeData.onImage === "function") {
                nodeData.onImage(e.target.result);
            }
        };

        reader.readAsDataURL(file);
    }

    // Button → file dialog
    button.addEventListener("click", () => {
        fileInput.click();
    });

    fileInput.addEventListener("change", (e) => {
        handleImage(e.target.files[0]);
    });

    // Drag & drop
    dropZone.addEventListener("dragover", (e) => {
        e.preventDefault();
        dropZone.style.borderColor = "white";
    });

    dropZone.addEventListener("dragleave", () => {
        dropZone.style.borderColor = "#888";
    });

    dropZone.addEventListener("drop", (e) => {
        e.preventDefault();
        dropZone.style.borderColor = "#888";

        const file = e.dataTransfer.files[0];
        handleImage(file);
    });
    
}










export async function startAndEvents(){
    wasm = await initWasm();
    console.log("hiiiiiiii");
    //wasm.start();

    document.addEventListener("nodeParamChange", (e) => {
      sendParam(e.detail.nodeId, e.detail.value, e.detail.argumentIndex);
    });

    buildCategoryDropdowns();
    document.querySelectorAll(".drag-drawflow").forEach(el => {
        el.draggable = true;
        el.addEventListener("dragstart", drag);
    });

    var id = document.getElementById("drawflow");
    window.editor = new Drawflow(id);
    editor.reroute = true;
    editor.start();


    const el = document.getElementById("drawflow");

    el.addEventListener("dragover", allowDrop);
    el.addEventListener("drop", drop);

    // Events!
    editor.on('nodeCreated', function(id) {
      console.log("Node created " + id);
    })

    editor.on('nodeRemoved', function(id) {
      console.log("Node removed " + id);
    })

    editor.on('nodeSelected', function(id) {
      console.log("Node selected " + id);
    })

    editor.on("nodeDataChanged", function(data) {
      console.log("Edit Data");
      console.log(data);
    });

    editor.on('moduleCreated', function(name) {
      console.log("Module Created " + name);
    })

    editor.on('moduleChanged', function(name) {
      console.log("Module Changed " + name);
    })

    editor.on('connectionCreated', function(connection) {
      console.log('Connection created');
      console.log(connection);
    })

    editor.on('connectionRemoved', function(connection) {
      console.log('Connection removed');
      console.log(connection);
    })

    /*
    editor.on('mouseMove', function(position) {
      console.log('Position mouse x:' + position.x + ' y:'+ position.y);
    })
    editor.on('translate', function(position) {
      console.log('Translate x:' + position.x + ' y:'+ position.y);
    })
      */

    editor.on('nodeMoved', function(id) {
      console.log("Node moved " + id);
    })

    editor.on('zoom', function(zoom) {
      console.log('Zoom level ' + zoom);
    })


    editor.on('addReroute', function(id) {
      console.log("Reroute added " + id);
    })

    editor.on('removeReroute', function(id) {
      console.log("Reroute removed " + id);
    })

    /* DRAG EVENT */

    /* Mouse and Touch Actions */

    var elements = document.getElementsByClassName('drag-drawflow');
    for (var i = 0; i < elements.length; i++) {
      elements[i].addEventListener('touchend', drop, false);
      //elements[i].addEventListener('touchmove', positionMobile, false);
      elements[i].addEventListener('touchstart', drag, false );
    }


    loadImageFromPath("../assets/len.png");
}


window.addEventListener('DOMContentLoaded', () => {
    startAndEvents();
});