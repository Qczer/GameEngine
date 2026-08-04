#pragma once

#include "GameEngine/Core/Core.hpp"
#include "GameEngine/Core/Timestep.hpp"
#include "GameEngine/Events/Event.hpp"

namespace GameEngine
{

class Layer
{
public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(Timestep /*ts*/) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& /*event*/) {}

  const std::string& GetName() const { return m_DebugName; }

protected:
  std::string m_DebugName;
};

} // namespace GameEngine
