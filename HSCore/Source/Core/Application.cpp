#include "Application.h"

namespace HSFramework
{
	Application::Application(const ApplicationInfo& info)
		: m_Info(info)
	{
		// TODO: Assert !s_Instance
	}

	Application::~Application()
	{
		// TODO: Assert this == s_Instance
		s_Instance = nullptr;
	}

	void Application::Run()
	{
		if (m_Running)
		{
			// TODO: Warn
			return;
		}

		m_Running = true;

		while (m_Running)
		{
			
		}

		CleanUp();
	}

	void Application::CleanUp()
	{
	}

	void Application::Quit()
	{
		if (!m_Running)
		{
			// TODO: Warn
			return;
		}

		m_Running = false;
	}
}
