const {
  getPlaybackDevices,
  getCaptureDevices,
  addInputDevice,
  getUpdates,
} = require("../../AudioMixerEngine/build/Release/AudioMixerEngine.node");

import { ipcMain } from "electron";
import { mainWindow } from "./main";

ipcMain.handle("audio:getPlaybackDevices", () => {
  return getPlaybackDevices();
});

ipcMain.handle("audio:getCaptureDevices", () => {
  return getCaptureDevices();
});

ipcMain.handle("audio:addInputDevice", (_event, id: string) => {
  return addInputDevice(id);
});

getUpdates((updates: any) =>
  mainWindow.webContents.send("audio:update", updates),
);
