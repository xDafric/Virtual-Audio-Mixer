const {
  getPlaybackDevices,
  getCaptureDevices,
  addChannel,
  removeChannel,
  setChannelDevice,
  setChannelName,
  setChannelVolume,
  getUpdates,
} = require('../../AudioMixerEngine/build/Release/AudioMixerEngine.node')

import { ipcMain } from 'electron'
import { mainWindow } from './main'

ipcMain.handle('audioMixer:getPlaybackDevices', () => {
  return getPlaybackDevices()
})

ipcMain.handle('audioMixer:getCaptureDevices', () => {
  return getCaptureDevices()
})

ipcMain.handle('audioMixer:addChannel', (_event, name: string) => {
  return addChannel(name)
})

ipcMain.handle('audioMixer:removeChannel', (_event, index: number) => {
  return removeChannel(index)
})

ipcMain.handle(
  'audioMixer:setChannelDevice',
  (_event, index: number, name: string) => {
    return setChannelDevice(index, name)
  }
)

ipcMain.handle(
  'audioMixer:setChannelName',
  (_event, index: number, name: string) => {
    return setChannelName(index, name)
  }
)

ipcMain.handle(
  'audioMixer:setChannelVolume',
  (_event, index: number, volume: number) => {
    return setChannelVolume(index, volume)
  }
)

getUpdates((updates: any) =>
  mainWindow.webContents.send('audioMixer:update', updates)
)
