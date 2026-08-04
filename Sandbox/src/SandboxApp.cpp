#include "GameEngine/Core/EntryPoint.hpp"
#include "GameEngine.hpp"

#include "ExampleLayer.hpp"
#include "Sandbox2D.hpp"

class Sandbox : public GameEngine::Application
{
public:
  Sandbox()
  {
    // PushLayer(new ExampleLayer());
    PushLayer(new Sandbox2D());
  }

  ~Sandbox() {}
};

GameEngine::Application* GameEngine::CreateApplication() { return new Sandbox(); }
