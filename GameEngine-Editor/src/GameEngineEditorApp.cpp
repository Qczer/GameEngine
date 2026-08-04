#include "GameEngine/Core/EntryPoint.hpp"
#include "GameEngine.hpp"

#include "EditorLayer.hpp"

namespace GameEngine
{

class GameEngineEditor : public Application
{
public:
  GameEngineEditor() : Application("GameEngine Editor") { PushLayer(new EditorLayer()); }

  ~GameEngineEditor() override = default;
};

Application* CreateApplication() { return new GameEngineEditor(); }
} // namespace GameEngine
