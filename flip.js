// address of native addon
const { FocusWindow } = require('./build/Release/addon.node');
const { ipcRenderer } = require ("electron");

document.getElementById("ashome1_btn").addEventListener("click", ()=> {
    flip("ashome1")}, false);
document.getElementById("ashome2_btn").addEventListener("click", ()=> {
    flip("ashome2")}, false);
document.getElementById("asrun1_btn").addEventListener("click", ()=> {
    flip("asrun1")}, false);
document.getElementById("asrun2_btn").addEventListener("click", ()=> {
    flip("asrun2")}, false);


let buttons = document.getElementsByTagName('button');
for (btn of buttons) {
    btn.addEventListener("mouseenter", () => {
        ipcRenderer.send("ct-off");
    })
    btn.addEventListener("mouseleave", () => {
        ipcRenderer.send("ct-on");
    })
}
/*
let sel = document.getElementById('sel_options');
sel.addEventListener("mouseenter", () => {
    ipcRenderer.send("ct-off");
});

sel.addEventListener("mouseleave", () => {
    ipcRenderer.send("ct-on");
});

sel.addEventListener("change", (event)=> {
    //console.log(event.target.value);
    flip(event.target.value);
});
*/
function flip(appName) {
    let runPath = "C:\\Titan 1.4\\autoStainerRun.exe";
    switch(appName) {
        case "ashome1":
            runPath = "C:\\Titan 1.4\\AShome.exe";
            break;
        case "ashome2":
            runPath = "C:\\Titan-S 1.4\\AShome.exe";
            break;
        case "asrun1":
            runPath = "C:\\Titan 1.4\\autoStainerRun.exe";
            break;
        case "asrun2":
            runPath = "C:\\Titan-S 1.4\\autoStainerRun.exe";
            break;
        default:
            runPath = "C:\\Titan 1.4\\autoStainerRun.exe";
    }

    FocusWindow(runPath);
}