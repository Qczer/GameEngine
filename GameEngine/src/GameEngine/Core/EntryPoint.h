#pragma once

#include "GameEngine/Core/Core.h"
#include "GameEngine/Core/Application.h"

#if defined(GE_PLATFORM_WINDOWS) || defined(GE_PLATFORM_LINUX)

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int argc, char** argv)
{
	GameEngine::Log::Init();

	GE_PROFILE_BEGIN_SESSION("Startup", "profiling/GameEngineProfile-Startup.json");
	auto app = GameEngine::CreateApplication();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Runtime", "profiling/GameEngineProfile-Runtime.json");
	app->Run();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Shutdown", "profiling/GameEngineProfile-Shutdown.json");
	delete app;
	GE_PROFILE_END_SESSION();
}

#endif