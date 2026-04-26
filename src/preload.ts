// See the Electron documentation for details on how to use preload scripts:
// https://www.electronjs.org/docs/latest/tutorial/process-model#preload-scripts
import { contextBridge, ipcRenderer } from 'electron'

contextBridge.exposeInMainWorld('audioMixer', {
  getPlaybackDevices: () => ipcRenderer.invoke('audioMixer:getPlaybackDevices'),
  getCaptureDevices: () => ipcRenderer.invoke('audioMixer:getCaptureDevices'),
  addChannel: (name: string) =>
    ipcRenderer.invoke('audioMixer:addChannel', name),
  removeChannel: (index: number) =>
    ipcRenderer.invoke('audioMixer:removeChannel', index),
  setChannelDevice: (index: number, name: string) =>
    ipcRenderer.invoke('audioMixer:setChannelDevice', index, name),
  setChannelName: (index: number, name: string) =>
    ipcRenderer.invoke('audioMixer:setChannelName', index, name),
  setChannelVolume: (index: number, volume: number) =>
    ipcRenderer.invoke('audioMixer:setChannelVolume', index, volume),
  getUpdates: (callback: (update: any) => void) => {
    ipcRenderer.on('audioMixer:update', (_event, data) => callback(data))
  },
})
