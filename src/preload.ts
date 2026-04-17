// See the Electron documentation for details on how to use preload scripts:
// https://www.electronjs.org/docs/latest/tutorial/process-model#preload-scripts
import { contextBridge, ipcRenderer } from "electron";

contextBridge.exposeInMainWorld("audio", {
  getPlaybackDevices: () => ipcRenderer.invoke("audio:getPlaybackDevices"),
  getCaptureDevices: () => ipcRenderer.invoke("audio:getCaptureDevices"),
  addInputDevice: (id: string) =>
    ipcRenderer.invoke("audio:addInputDevice", id),
});
