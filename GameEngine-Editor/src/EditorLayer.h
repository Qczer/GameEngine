#pragma once

#include "GameEngine.h"

namespace GameEngine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(GameEngine::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(GameEngine::Event& e) override;
	private:
		GameEngine::OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		float m_FrameRate;
	};

}