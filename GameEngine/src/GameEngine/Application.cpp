#include "Application.h"

#include "GameEngine/Events/ApplicationEvent.h"
#include "GameEngine/Log.h"

namespace GameEngine {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run() const
	{
		WindowResizeEvent e(1280, 720);

		if(e.isInCategory(EventCategoryApplication))
		{
			GE_TRACE(e); // gray
		}
		if(e.isInCategory(EventCategoryInput))
		{
			GE_INFO(e); // green
		}
	}

}