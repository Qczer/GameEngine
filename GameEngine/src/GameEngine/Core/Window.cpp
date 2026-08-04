#include "gepch.hpp"
#include "GameEngine/Core/Window.hpp"

#ifdef GE_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.hpp"
#elif defined(GE_PLATFORM_LINUX)
#include "Platform/Linux/LinuxWindow.hpp"
#endif

namespace GameEngine
{

Scope<Window> Window::Create(const WindowProps& props)
{
#ifdef GE_PLATFORM_WINDOWS
  return CreateScope<WindowsWindow>(props);
#elif defined(GE_PLATFORM_LINUX)
  return CreateScope<LinuxWindow>(props);
#else
  GE_CORE_ASSERT(false, "Unknown platform!");
  return nullptr;
#endif
}

} // namespace GameEngine
