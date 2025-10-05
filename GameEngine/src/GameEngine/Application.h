#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace GameEngine {

	class GAME_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run() const;

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in client
	Application* CreateApplication();

}