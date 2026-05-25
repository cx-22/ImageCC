import { addNodeToDrawFlow } from "./nodes.js";
import { buildDropdowns } from "./dataflow-element.js";

/* =========================
   Shared state
========================= */

let mobile_item_selec = "";
let mobile_last_move = null;
let transform = "";

/* =========================
   GLOBAL FALLBACKS (for inline HTML if still used)
========================= */

window.allowDrop = function (ev) {
    ev.preventDefault();
};

window.drag = drag;
window.drop = drop;
window.positionMobile = positionMobile;

/* =========================
   MAIN ENTRY
========================= */

export function main() {

    const wrapper = document.querySelector(".col");

    const items = [
        { name: "Input Image", category: "Base", node: "input" },
        { name: "Output Image", category: "Base", node: "output" },
        { name: "Addition", category: "Basic Operation", node: "add" }
    ];

    buildDropdowns(items, wrapper);

    initDnD();
}

/* =========================
   DnD INIT
========================= */

function initDnD() {

    const elements = document.getElementsByClassName("drag-drawflow");

    for (let i = 0; i < elements.length; i++) {
        elements[i].addEventListener("touchend", drop, false);
        elements[i].addEventListener("touchmove", positionMobile, false);
        elements[i].addEventListener("touchstart", drag, false);

        elements[i].addEventListener("dragstart", drag, false);
    }
}

/* =========================
   EVENTS
========================= */

function positionMobile(ev) {
    mobile_last_move = ev;
}

function drag(ev) {

    if (ev.type === "touchstart") {

        mobile_item_selec = ev.target
            .closest(".drag-drawflow")
            ?.getAttribute("data-node");

    } else {
        ev.dataTransfer.setData(
            "node",
            ev.target.getAttribute("data-node")
        );
    }
}

function drop(ev) {

    if (ev.type === "touchend") {

        const point = mobile_last_move?.touches?.[0];

        if (!point) return;

        const parentdrawflow =
            document
                .elementFromPoint(point.clientX, point.clientY)
                ?.closest("#drawflow");

        if (parentdrawflow != null) {
            addNodeToDrawFlow(
                mobile_item_selec,
                point.clientX,
                point.clientY
            );
        }

        mobile_item_selec = "";

    } else {

        ev.preventDefault();

        const data = ev.dataTransfer.getData("node");

        addNodeToDrawFlow(data, ev.clientX, ev.clientY);
    }
}

/* =========================
   OPTIONAL UI HELPERS (not used yet)
========================= */

function showpopup(e) {

    e.target.closest(".drawflow-node").style.zIndex = "9999";
    e.target.children[0].style.display = "block";

    transform = editor?.precanvas?.style?.transform || "";

    editor.precanvas.style.transform = "";
    editor.precanvas.style.left = editor.canvas_x + "px";
    editor.precanvas.style.top = editor.canvas_y + "px";

    editor.editor_mode = "fixed";
}

function closemodal(e) {

    e.target.closest(".drawflow-node").style.zIndex = "2";
    e.target.parentElement.parentElement.style.display = "none";

    editor.precanvas.style.transform = transform;
    editor.precanvas.style.left = "0px";
    editor.precanvas.style.top = "0px";

    editor.editor_mode = "edit";
}

function changeModule(event) {

    const all = document.querySelectorAll(".menu ul li");

    for (let i = 0; i < all.length; i++) {
        all[i].classList.remove("selected");
    }

    event.target.classList.add("selected");
}

function changeMode(option) {

    if (option === "lock") {
        lock.style.display = "none";
        unlock.style.display = "block";
    } else {
        lock.style.display = "block";
        unlock.style.display = "none";
    }
}

    function drop(ev) {
      if (ev.type === "touchend") {
        var parentdrawflow = document.elementFromPoint( mobile_last_move.touches[0].clientX, mobile_last_move.touches[0].clientY).closest("#drawflow");
        if(parentdrawflow != null) {
          addNodeToDrawFlow(mobile_item_selec, mobile_last_move.touches[0].clientX, mobile_last_move.touches[0].clientY);
        }
        mobile_item_selec = '';
      } else {
        ev.preventDefault();
        var data = ev.dataTransfer.getData("node");
        addNodeToDrawFlow(data, ev.clientX, ev.clientY);
      }

    }