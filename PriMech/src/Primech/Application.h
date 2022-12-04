#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace PriMech {
	class PRIMECH_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> pWindow_;
		bool running_ = true;
	};

	//defined in client [e.g Sandbox]
	Application* CreateApplication();
}