#pragma once

#include "GameEngine.hpp"
#include "Panels/ContentBrowserPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp"

#include "GameEngine/Events/KeyEvent.hpp"
#include "GameEngine/Renderer/EditorCamera.hpp"
#include "GameEngine/Scene/SceneSerializer.hpp"

#include <filesystem>

namespace GameEngine
{

class EditorLayer final : public Layer
{
public:
  EditorLayer();
  ~EditorLayer() override = default;

  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate(Timestep ts) override;
  void OnImGuiRender() override;
  void OnEvent(Event& e) override;

private:
  bool OnKeyPressed(KeyPressedEvent& e);
  bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

  void NewScene();
  void OpenScene();
  void OpenScene(const std::filesystem::path& path);
  void SaveSceneAs();

  void OnScenePlay();
  void OnSceneStop();

  // UI Panels
  void UI_Toolbar();

private:
  OrthographicCameraController m_CameraController;

  // Temp
  Ref<VertexArray> m_SquareVA;
  Ref<Shader> m_FlatColorShader;
  Ref<Framebuffer> m_Framebuffer;
  Ref<Texture2D> m_Texture;
  Ref<Scene> m_ActiveScene;

  EditorCamera m_EditorCamera;
  std::optional<Entity> m_HoveredEntity;

  float m_FrameRate = 0.0f;
  float m_ElapsedTime = 0.0f;

  bool m_ViewportFocused = false;
  bool m_ViewportHovered = false;
  bool m_GuizmoHovered = false;
  glm::vec2 m_ViewportSize = {0.0f, 0.0f};
  glm::vec2 m_ViewportBounds[2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
  glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
  int m_GuizmoType = 7;

  enum class SceneState
  {
    Edit = 0,
    Play = 1
  };
  SceneState m_SceneState = SceneState::Edit;

  // Panels
  SceneHierarchyPanel m_SceneHierarchyPanel;
  ContentBrowserPanel m_ContentBrowserPanel;

  // Editor resources
  Ref<Texture2D> m_IconPlay, m_IconStop;
};

} // namespace GameEngine
