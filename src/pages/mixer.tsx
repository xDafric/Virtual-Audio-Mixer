import Channel, { type ChannelType } from '@/components/channel'
import NewChannel from '@/components/newChannel'
import { useEffect, useState } from 'react'

const Mixer = () => {
  const [lines, setLines] = useState<ChannelType[]>([])

  useEffect(() => {
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
        }))
      )
    })
  }, [])

  return (
    <>
      <div className="flex h-full w-full flex-row items-end gap-4 overflow-hidden">
        {lines.map((line, index) => (
          <Channel key={index} index={index} channel={line} />
        ))}
        <NewChannel
          onClick={() =>
            window.audioMixer.addChannel(`Channel ${lines.length}`)
          }
        />
      </div>
    </>
  )
}

export default Mixer
