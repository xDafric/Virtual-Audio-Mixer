import { Outlet } from 'react-router-dom'
import Sidebar from './components/sidebar'

const Layout = () => {
  return (
    <>
      <div className="h-screen w-screen overflow-hidden bg-muted">
        <div className="drag absolute top-0 left-0 flex h-10 w-full flex-row items-center bg-muted pl-4">
          <h1 className="text-sm font-normal text-primary/80">
            Virtual Audio Mixer
          </h1>
        </div>
        <Sidebar />
        <div className="h-full w-full pt-10 pl-14">
          <div className="h-full w-full rounded-tl-xl border-t-2 border-l-2 border-white/15 bg-background p-4">
            <Outlet />
          </div>
        </div>
      </div>
    </>
  )
}

export default Layout
