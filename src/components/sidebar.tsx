import { cn } from '@/lib/utils'
import { Cpu, SlidersVertical, type LucideIcon } from 'lucide-react'
import { NavLink } from 'react-router-dom'

const items: { icon: LucideIcon; link: string }[] = [
  { icon: SlidersVertical, link: '/' },
  { icon: Cpu, link: '/device' },
]

const Sidebar = () => {
  return (
    <>
      <div className="absolute top-10 bottom-0 left-0 flex w-14 flex-col items-center gap-2 bg-muted pt-2">
        {items.map((item) => (
          <NavLink
            to={item.link}
            className={({ isActive, isPending }) =>
              cn(
                'flex h-11 w-11 cursor-pointer items-center justify-center rounded-md duration-100 hover:bg-white/10',
                {
                  'border-2 border-[#005c56] bg-[#00726b] hover:border-[#00413c] hover:bg-[#005c56]':
                    isActive,
                }
              )
            }
          >
            <item.icon />
          </NavLink>
        ))}
      </div>
    </>
  )
}

export default Sidebar
