import DeviceDropdown from "./deviceDropdown";

export type ChannelType = {
  name: string;
  device: string;
  volume: number;
  left: number;
  right: number;
};

export interface ChannelProps {
  channel: ChannelType;
  index: number;
}

const Channel: React.FC<ChannelProps> = (props) => {
  return (
    <div
      className="flex flex-row items-end gap-2 h-full"
    >
      <input
        type="range"
        value={props.channel.volume}
        max={1}
        min={0}
        step={0.01}
        onChange={(value) =>
          window.audioMixer.setChannelVolume(
            props.index,
            parseFloat(value.target.value),
          )
        }
      />
      <DeviceDropdown
        type="input"
        value={props.channel.device}
        onValueChange={(value) =>
          window.audioMixer.setChannelDevice(props.index, value)
        }
      />
      <div
        className="w-5 bg-green-500"
        style={{
          height: 500 * props.channel.left,
        }}
      ></div>
      <div
        className="w-5 bg-green-500"
        style={{
          height: 500 * props.channel.right,
        }}
      ></div>
    </div>
  );
};

export default Channel;
