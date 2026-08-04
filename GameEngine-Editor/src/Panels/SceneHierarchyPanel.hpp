#pragma once

#include "GameEngine/Core/Core.hpp"
#include "GameEngine/Scene/Entity.hpp"
#include "GameEngine/Scene/Scene.hpp"

namespace GameEngine
{

class SceneHierarchyPanel
{
public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(const Ref<Scene>& scene);

  void SetContext(const Ref<Scene>& scene);

  void OnImGuiRender();

  void SetSelectedEntity(Entity entity);
  void ResetSelectedEntity();
  [[nodiscard]] Entity GetSelectedEntity() const { return m_SelectionContext; }

private:
  void DrawEntityNode(Entity& entity);
  void DrawComponents(Entity& entity);

private:
  Ref<Scene> m_Context;
  Entity m_SelectionContext;
};

} // namespace GameEngine
