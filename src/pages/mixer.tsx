import Channel, { type ChannelType } from '@/components/channel'
import { Button } from '@/components/ui/button'
import { useEffect, useState } from 'react'

const Mixer = () => {
  const [lines, setLines] = useState<ChannelType[]>([])

  useEffect(() => {
    //window.audioMixer.addChannel("Test");

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
      <Button>Test</Button>

      <div className="flex h-125 flex-row items-end gap-12.5">
        {lines.map((line, index) => (
          <Channel key={index} index={index} channel={line} />
        ))}
      </div>
    </>
  )
}

export default Mixer
