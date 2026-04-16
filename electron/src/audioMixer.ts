import { createRequire } from "module";
const require = createRequire(import.meta.url);

const {
  getPlaybackDevices,
  getCaptureDevices,
  addInputDevice,
} = require("../../AudioMixerEngine/build/Release/AudioMixerEngine.node");

class AudioMixer {
  constructor() {}
  addInputDevice(id: number) {
    return addInputDevice(id);
  }
  getPlaybackDevices() {
    return getPlaybackDevices();
  }
  getCaptureDevices() {
    return getCaptureDevices();
  }
}

export default new AudioMixer();
