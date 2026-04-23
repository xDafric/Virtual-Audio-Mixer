import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { HashRouter, Route, Routes } from 'react-router-dom'
import { ThemeProvider } from './components/theme-provider'
import Layout from './layout'
import Device from './pages/device'
import Mixer from './pages/mixer'

export default function App() {
  return (
    <HashRouter>
      <Routes>
        <Route element={<Layout></Layout>}>
          <Route path="/" element={<Mixer />} />
          <Route path="/device" element={<Device />} />
        </Route>
      </Routes>
    </HashRouter>
  )
}

const container = document.getElementById('app')
if (!container) {
  throw new Error('Root element with id "app" not found')
}
const root = createRoot(container)
root.render(
  <StrictMode>
    <ThemeProvider>
      <App />
    </ThemeProvider>
  </StrictMode>
)
