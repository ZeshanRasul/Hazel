#include "hzpch.h"
#include "Application.h"

#include "Hazel/Events/MouseEvent.h"
#include "Log.h"

namespace Hazel {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		MouseButtonPressedEvent e(0);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			HZ_TRACE(e);
		}


		while (true);
	}

}
