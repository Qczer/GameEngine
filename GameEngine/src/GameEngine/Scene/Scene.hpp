#pragma once

#include <entt.hpp>

#include "GameEngine/Core/Timestep.hpp"
#include "GameEngine/Renderer/EditorCamera.hpp"

namespace GameEngine
{

class Entity;

class Scene
{
public:
  Scene() = default;
  ~Scene() = default;

  Entity CreateEntity(const std::string& name = std::string());
  void DestroyEntity(Entity entity);

  void OnUpdateRuntime(Timestep ts);
  void OnUpdateEditor(Timestep ts, const EditorCamera& camera);
  void OnViewportResize(uint32_t width, uint32_t height);

  Entity GetPrimaryCameraEntity();

private:
  template <typename T> void OnComponentAdded(Entity& /*entity*/, T& /*component*/) {}

private:
  entt::registry m_Registry;
  uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

  friend class Entity;
  friend class SceneSerializer;
  friend class SceneHierarchyPanel;
};

} // namespace GameEngine
