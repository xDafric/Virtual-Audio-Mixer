export {}

declare global {
  interface Window {
    audioMixer: {
      getPlaybackDevices: () => Promise<any>
      getCaptureDevices: () => Promise<any>
      addChannel: (name: string) => void
      removeChannel: (index: number) => void
      setChannelDevice: (index: number, name: string) => void
      setChannelName: (index: number, name: string) => void
      setChannelVolume: (index: number, volume: number) => void
      getUpdates: (callback: (update: any) => void) => void
    }
  }
}
