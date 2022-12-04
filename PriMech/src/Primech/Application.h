#pragma once
#include "Core.h"
#include "PriMech/Events/Event.h"
#include "PriMech/LayerStack/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace PriMech {
	class PRIMECH_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> pWindow_;
		bool running_ = true;
		LayerStack layerStack_;
	};

	//defined in client [e.g Sandbox]
	Application* CreateApplication();
}