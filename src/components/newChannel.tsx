import { Plus } from 'lucide-react'

const NewChannel = ({ onClick }: { onClick?: () => void }) => {
  return (
    <>
      <div
        onClick={onClick}
        className="group flex h-full w-55 cursor-pointer flex-col items-center justify-center gap-2 rounded-md border-2 border-dashed border-muted/80 duration-100 hover:bg-muted/50"
      >
        <span className="font-semibold text-white/40 group-hover:text-white/60">
          New Channel
        </span>
        <Plus size={48} className="text-white/40 group-hover:text-white/60" />
      </div>
    </>
  )
}

export default NewChannel
