#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace PriMech {
	class PRIMECH_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

	private:
		std::unique_ptr<Window> pWindow_;
		bool running_ = true;
	};

	// to be defined in client
	Application* CreateApplication();
}