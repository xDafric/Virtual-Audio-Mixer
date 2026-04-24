import {
  Popover,
  PopoverContent,
  PopoverTrigger,
} from '@/components/ui/popover'
import { useEffect, useState } from 'react'
export interface DeviceDropdownProps {
  type: 'input' | 'output'
  value: string
  onValueChange: (value: string) => void
}

const DeviceDropdown: React.FC<DeviceDropdownProps> = (props) => {
  const [devices, setDevices] = useState<{ name: 'string' }[]>([])
  const [open, setOpen] = useState(false)

  const fetchPlaybackDevices = async () => {
    const devices = await window.audioMixer.getPlaybackDevices()
    setDevices(devices)
  }

  const fetchCaptureDevices = async () => {
    const devices = await window.audioMixer.getCaptureDevices()
    setDevices(devices)
  }

  useEffect(() => {
    if (props.type == 'input') {
      fetchCaptureDevices()
    } else {
      fetchPlaybackDevices()
    }
  }, [])

  return (
    <>
      <Popover open={open} onOpenChange={(open) => setOpen(open)}>
        <PopoverTrigger className="w-full">
          <span className="block w-full cursor-pointer overflow-hidden text-sm text-nowrap text-ellipsis text-white/60 hover:underline">
            {props.value}
          </span>
        </PopoverTrigger>
        <PopoverContent align="start" className="w-60">
          <div className="flex flex-col gap-1">
            {devices.map((device, index) => (
              <div
                className="cursor-pointer rounded-sm p-2 duration-100 hover:bg-muted"
                key={index}
                onClick={() => {
                  setOpen(false)
                  props.onValueChange(device.name)
                }}
              >
                {device.name}
              </div>
            ))}
          </div>
        </PopoverContent>
      </Popover>
    </>
  )
}

export default DeviceDropdown
