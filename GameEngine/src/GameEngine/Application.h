#pragma once

#include "Core.h"

namespace GameEngine {

	class GAME_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run() const;
	};

	// To be defined in client
	Application* CreateApplication();

}