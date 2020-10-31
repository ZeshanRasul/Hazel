#pragma once

#include "Core.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public: 
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client, we just need the declaration, linker will find the definition later
	Application* CreateApplication();
}


