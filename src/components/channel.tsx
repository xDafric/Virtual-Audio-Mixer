import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu'
import { cn } from '@/lib/utils'
import { ChevronDown, Trash2 } from 'lucide-react'
import { useState } from 'react'
import DeviceDropdown from './deviceDropdown'
import { Separator } from './ui/separator'

export type ChannelType = {
  name: string
  device: string
  volume: number
  left: number
  right: number
}

export interface ChannelProps {
  channel: ChannelType
  index: number
}

const Channel: React.FC<ChannelProps> = (props) => {
  const [dragging, setDragging] = useState(false)
  const [open, setOpen] = useState(false)

  const accentColors = [
    'bg-red-700/90 border-red-900',
    'bg-blue-700/90 border-blue-900',
    'bg-green-700/90 border-green-900',
    'bg-purple-700/90 border-purple-900',
    'bg-pink-700/90 border-pink-900',
    'bg-orange-700/90 border-orange-900',
    'bg-teal-700/90 border-teal-900',
  ]

  const color = accentColors[props.index % accentColors.length]

  return (
    <div className="flex h-full w-55 flex-col items-end gap-2 rounded-md border-2 border-muted/80 bg-muted/50">
      <div className="flex w-full flex-col gap-3">
        <div
          className={cn(
            `flex h-11 w-full flex-row items-center justify-center gap-2 rounded-sm border-2 pr-2 backdrop-blur-2xl`,
            color
          )}
        >
          <div className="flex aspect-square h-full cursor-grab items-center justify-center rounded-md border-2 border-neutral-700/60 bg-muted/60 shadow-[inset_0px_0px_10px_1px_#1212128f] backdrop-blur-3xl">
            <span className="text-white/60">#{props.index}</span>
          </div>
          <div className="flex w-full flex-row items-center gap-1 overflow-hidden">
            <input
              type="text"
              value={props.channel.name}
              onChange={(event) =>
                window.audioMixer.setChannelName(
                  props.index,
                  event.target.value
                )
              }
              className="w-full rounded-md border-2 border-transparent px-1 py-1 outline-0 focus:border-black/20"
            />

            <DropdownMenu open={open} onOpenChange={(open) => setOpen(open)}>
              <DropdownMenuTrigger className="cursor-pointer rounded-full p-1 duration-100 hover:bg-white/20">
                <ChevronDown
                  className={cn('duration-150', { 'rotate-180': open })}
                />
              </DropdownMenuTrigger>
              <DropdownMenuContent>
                <DropdownMenuItem
                  onClick={() => window.audioMixer.removeChannel(props.index)}
                >
                  <Trash2 />
                  Remove
                </DropdownMenuItem>
              </DropdownMenuContent>
            </DropdownMenu>
          </div>
        </div>
        <div className="flex h-min items-center justify-center gap-2 overflow-hidden px-2">
          <div className="h-1.5 w-1.5 shrink-0 rounded-full bg-green-500"></div>
          <div className="flex max-w-full min-w-0">
            <DeviceDropdown
              type="input"
              value={props.channel.device}
              onValueChange={(value) =>
                window.audioMixer.setChannelDevice(props.index, value)
              }
            />
          </div>
        </div>
        <Separator />
      </div>

      <div className="flex h-full w-full flex-col items-center justify-center py-12">
        <div className="relative flex h-full w-15 flex-col items-center justify-center">
          <div className="relative flex h-full w-5 flex-col items-center rounded-sm bg-background"></div>
          <div
            className={cn(
              'absolute z-10 h-8 w-11 gap-2 rounded-md border-2 border-transparent bg-muted transition-colors duration-200',
              {
                'scale-105 border-neutral-700': dragging,
              }
            )}
            style={{
              bottom: `calc(${props.channel.volume * 100}% - 16px)`,
              transitionProperty: 'scale',
            }}
          ></div>
          <div
            className="absolute bottom-0 w-5 overflow-hidden rounded-sm bg-green-500"
            style={{
              height: `${props.channel.left * 100}%`,
            }}
          ></div>
          <input
            type="range"
            min="0"
            max="1"
            step={0.01}
            value={props.channel.volume}
            onChange={(value) =>
              window.audioMixer.setChannelVolume(
                props.index,
                parseFloat(value.target.value)
              )
            }
            onMouseDown={() => setDragging(true)}
            onMouseUp={() => setDragging(false)}
            className="absolute z-20 h-full w-full cursor-pointer opacity-0"
            style={{ writingMode: 'sideways-lr' }}
          />
        </div>
      </div>
    </div>
  )
}

export default Channel
