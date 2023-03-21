const { app, BrowserWindow, ipcMain, dialog } = require('electron')
const path = require('path')

const createWindow = () => {
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        transparent:true,
        frame: false,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js'),
            nodeIntegration: true,
            nodeIntegrationInWorker: true,
            contextIsolation: false
        }
    });
    win.setAlwaysOnTop(true, 'screen');
    win.maximize();
    win.loadFile('index.html');

    ipcMain.on ("ct-off", (event, args) => {
        win.setIgnoreMouseEvents(false);
    });
    
    ipcMain.on ("ct-on", (event, args) => {
        win.setIgnoreMouseEvents(true, {forward: true});
    });

}


app.whenReady().then(() => {
    createWindow();
    // theres some macOS code but imma skip it
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin')
        app.quit();
});