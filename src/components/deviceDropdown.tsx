import { useEffect, useState } from "react";

export interface DeviceDropdownProps {
  type: "input" | "output";
  value: string;
  onValueChange: (value: string) => void;
}

const DeviceDropdown: React.FC<DeviceDropdownProps> = (props) => {
  const [devices, setDevices] = useState<{ name: "string" }[]>([]);

  const fetchPlaybackDevices = async () => {
    const devices = await window.audioMixer.getPlaybackDevices();
    setDevices(devices);
  };

  const fetchCaptureDevices = async () => {
    const devices = await window.audioMixer.getCaptureDevices();
    setDevices(devices);
  };

  useEffect(() => {
    if (props.type == "input") {
      fetchCaptureDevices();
    } else {
      fetchPlaybackDevices();
    }
  }, []);

  return (
    <>
      <select
        value={props.value}
        onChange={(value) => props.onValueChange(value.target.value)}
      >
        {devices.map((device, index) => (
          <option key={index} value={device.name}>
            {device.name}
          </option>
        ))}
      </select>
    </>
  );
};

export default DeviceDropdown;
