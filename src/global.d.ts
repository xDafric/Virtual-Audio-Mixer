export {};

declare global {
  interface Window {
    audio: {
      getPlaybackDevices: () => Promise<any>;
      getCaptureDevices: () => Promise<any>;
      addInputDevice: (id: number) => Promise<any>;
    };
  }
}
