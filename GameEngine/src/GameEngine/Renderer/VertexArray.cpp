#include "gepch.hpp"
#include "GameEngine/Renderer/VertexArray.hpp"

#include "GameEngine/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace GameEngine
{

Ref<VertexArray> VertexArray::Create()
{
  switch (Renderer::GetAPI())
  {
    case RendererAPI::API::None:
      GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexArray>();
  }

  GE_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}

} // namespace GameEngine
