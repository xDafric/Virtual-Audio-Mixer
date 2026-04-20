import { useEffect, useState } from "react";
import Channel, { ChannelType } from "./components/channel";

const Home = () => {
  const [lines, setLines] = useState<ChannelType[]>([]);

  useEffect(() => {
    window.audioMixer.addChannel("Test");

    window.audioMixer.getUpdates((update) => {
      setLines((prev) =>
        update.map((line: ChannelType, i: number) => ({
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
      );
    });
  }, []);

  return (
    <>
      <h1>Virtual Audio Mixer</h1>

      <div
        style={{
          display: "flex",
          flexDirection: "row",
          alignItems: "end",
          height: 500,
          gap: 50,
        }}
      >
        {lines.map((line, index) => (
          <Channel key={index} index={index} channel={line} />
        ))}
      </div>
    </>
  );
};

export default Home;
