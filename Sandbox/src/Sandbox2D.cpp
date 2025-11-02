#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <print>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	GE_PROFILE_FUNCTION();

	m_Texture = GameEngine::Texture2D::Create("assets/textures/Checkerboard.png");

	GameEngine::FramebufferSpecification framebufferSpec;
	framebufferSpec.Width = 1280;
	framebufferSpec.Height = 720;
}

void Sandbox2D::OnDetach()
{
	GE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(GameEngine::Timestep ts)
{
	GE_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);
	m_FrameRate = (float)ts;

	// Render
	GameEngine::Renderer2D::ResetStats();
	{
		GE_PROFILE_SCOPE("Renderer Prep");
		GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GameEngine::RenderCommand::Clear();
	}

	{
		GE_PROFILE_SCOPE("Renderer Draw");

		static float rotation = 0;
		rotation += ts * 50.0f;

		GameEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		GameEngine::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		GameEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		GameEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		GameEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, 20.0f);
		GameEngine::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_Texture, 20.0f);

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.75f };
				GameEngine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		GameEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	GE_PROFILE_FUNCTION();

	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		ImGui::Begin("Settings");

		auto stats = GameEngine::Renderer2D::GetStats();
		ImGui::Text("Framerate: %i", (int)(1.0f / m_FrameRate));
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}
}

void Sandbox2D::OnEvent(GameEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}