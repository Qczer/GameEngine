#include "Application.h"

#include <print>
#include <iostream>

namespace GameEngine {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run() const
	{
		std::println("Hello, its Sandbox!");
		std::cin.get();
	}

}