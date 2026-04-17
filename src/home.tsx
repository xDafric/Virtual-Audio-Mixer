import { useEffect, useState } from "react";

const Home = () => {
  const [playbackDevices, setPlaybackDevices] = useState([]);
  const [captureDevices, setCaptureDevices] = useState([]);

  const fetchPlaybackDevices = async () => {
    const devices = await window.audio.getPlaybackDevices();
    setPlaybackDevices(devices);
  };

  const fetchCaptureDevices = async () => {
    const devices = await window.audio.getCaptureDevices();
    setCaptureDevices(devices);
  };

  useEffect(() => {
    fetchPlaybackDevices();
    fetchCaptureDevices();
  }, []);

  return (
    <>
      <h1>Virtual Audio Mixer</h1>
      <h2>Playback Devices</h2>
      <button onClick={() => fetchPlaybackDevices()}>Refresh</button>
      <ul>
        {playbackDevices.map((device: { name: string; id: number }) => (
          <li key={device.id}>{device.name}</li>
        ))}
      </ul>

      <h2>Capture Devices</h2>
      <button onClick={() => fetchCaptureDevices()}>Refresh</button>
      <ul>
        {captureDevices.map((device: { name: string; id: number }) => (
          <li key={device.id}>
            {device.name}
            <button
              onClick={() => {
                window.audio.addInputDevice(device.id);
              }}
            >
              Add
            </button>
          </li>
        ))}
      </ul>
    </>
  );
};

export default Home;
