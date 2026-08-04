#include "gepch.hpp"
#include "GameEngine/Renderer/RenderCommand.hpp"

namespace GameEngine
{

Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
