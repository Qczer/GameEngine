#include "gepch.hpp"
#include "UniformBuffer.hpp"

#include "GameEngine/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLUniformBuffer.hpp"

namespace GameEngine
{

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
{
  switch (Renderer::GetAPI())
  {
    case RendererAPI::API::None:
      GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLUniformBuffer>(size, binding);
    default:
      GE_CORE_ASSERT(false, "Unknown RendererAPI!");
      return nullptr;
  }
}

} // namespace GameEngine
