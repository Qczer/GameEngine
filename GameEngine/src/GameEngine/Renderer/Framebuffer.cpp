#include "gepch.hpp"
#include "GameEngine/Renderer/Framebuffer.hpp"

#include "GameEngine/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace GameEngine
{

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
  switch (Renderer::GetAPI())
  {
    case RendererAPI::API::None:
      GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLFramebuffer>(spec);
  }

  GE_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}

} // namespace GameEngine
