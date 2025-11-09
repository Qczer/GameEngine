#include "gepch.h"
#include "Platform/Linux/LinuxWindow.h"

#include "GameEngine/Renderer/Renderer.h"

#include "GameEngine/Events/ApplicationEvent.h"
#include "GameEngine/Events/MouseEvent.h"
#include "GameEngine/Events/KeyEvent.h"

namespace GameEngine {

	float Window::s_HighDPIScaleFactor = 1.0f;

	static uint8_t s_GLFWInitialized = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GE_CORE_ERROR("GLFW Error ({}): {}", error, description);
	}

	LinuxWindow::LinuxWindow(const WindowProps& props)
	{
		GE_PROFILE_FUNCTION();

		Init(props);
	}

	LinuxWindow::~LinuxWindow()
	{
		GE_PROFILE_FUNCTION();

		Shutdown();
	}

	void LinuxWindow::Init(const WindowProps& props)
	{
		GE_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		GE_CORE_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

		if(s_GLFWInitialized == 0)
		{
			GE_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			GE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			GE_PROFILE_SCOPE("glfwCreateWindow");

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			float xscale, yscale;
			glfwGetMonitorContentScale(monitor, &xscale, &yscale);

			if (xscale > 1.0f || yscale > 1.0f)
			{
				s_HighDPIScaleFactor = yscale;
				glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
			}

			#if defined(GE_BUILD_DEBUG)
				if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
					glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWInitialized;
		}
		
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(static_cast<KeyCode>(key), 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(static_cast<KeyCode>(key));
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(static_cast<KeyCode>(key), 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch(action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void LinuxWindow::Shutdown()
	{
		GE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWInitialized;

		if (s_GLFWInitialized == 0)
		{
			glfwTerminate();
		}
	}

	void LinuxWindow::OnUpdate()
	{
		GE_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void LinuxWindow::SetVSync(bool enabled)
	{
		GE_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool LinuxWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}