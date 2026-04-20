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
      style={{
        display: "flex",
        flexDirection: "row",
        alignItems: "end",
        gap: 10,
        height: "100%",
      }}
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
        style={{
          height: 500 * props.channel.left,
          width: 20,
          backgroundColor: "green",
        }}
      ></div>
      <div
        style={{
          height: 500 * props.channel.right,
          width: 20,
          backgroundColor: "green",
        }}
      ></div>
    </div>
  );
};

export default Channel;
