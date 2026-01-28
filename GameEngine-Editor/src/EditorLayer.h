#pragma once

#include "GameEngine.h"
#include "Panels/SceneHierarchyPanel.h"

#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Renderer/EditorCamera.h"

namespace GameEngine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Entity m_HoveredEntity;

		EditorCamera m_EditorCamera;

		Ref<Scene> m_ActiveScene;
		Ref<Texture2D> m_Texture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GuizmoType = 7;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		float m_FrameRate = 0.0f;
		float m_ElapsedTime = 0.0f;
	};

}