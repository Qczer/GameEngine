#pragma once

#include "GameEngine/Core/Core.h"

#ifdef GE_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int argc, char** argv)
{
	GameEngine::Log::Init();
	GE_CORE_WARN("Initialized Log!");
	GE_INFO("Hello!");

	auto app = GameEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif