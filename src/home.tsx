import { useEffect, useState } from "react";

const Home = () => {
  const [playbackDevices, setPlaybackDevices] = useState([]);
  const [captureDevices, setCaptureDevices] = useState([]);

  const [lines, setLines] = useState<
    { lineId: number; left: number; right: number }[]
  >([]);

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

    window.audio.getUpdates((update) =>
      setLines((prev) =>
        update.map((line: any, i: number) => ({
          ...line,
          left:
            line.left > (prev[i]?.left ?? 0)
              ? line.left
              : (prev[i]?.left ?? 0) - 0.001,
          right:
            line.right > (prev[i]?.right ?? 0)
              ? line.right
              : (prev[i]?.right ?? 0) - 0.001,
        })),
      ),
    );
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

      <div
        style={{
          display: "flex",
          flexDirection: "row",
          alignItems: "end",
          height: 500,
          gap: 50,
        }}
      >
        {lines.map((line) => (
          <div
            key={line.lineId}
            style={{ display: "flex", flexDirection: "row", gap: 10 }}
          >
            <div
              style={{
                height: 500 * line.left,
                width: 20,
                backgroundColor: "green",
              }}
            ></div>
            <div
              style={{
                height: 500 * line.right,
                width: 20,
                backgroundColor: "green",
              }}
            ></div>
          </div>
        ))}
      </div>
    </>
  );
};

export default Home;
