#include "ppch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"

#include <glad/glad.h>

namespace PriMech {
	
#define BIND_EVENT_FUNCTION(x)	std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		pWindow_ = std::unique_ptr<Window>(Window::Create());
		pWindow_->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
		PM_CORE_INFO("CONSTUCTOR CALLED FOR APPLICATION");

		unsigned int id;
		glGenVertexArrays(1, &id);
	}

	Application::~Application() {}

	void Application::PushLayer(Layer* layer) {
		layerStack_.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		layerStack_.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));

		//Printing all events for debugging purposes
		if (event.IsInCategory(EventCategoryApplication) ||
			event.IsInCategory(EventCategoryKeyboard)) {
			PM_CORE_TRACE(event); //keyboard and window events in white
		}
		else {
			PM_CORE_DEBUG(event); //mouse events in blue
		}

		for (auto iterator = layerStack_.end(); iterator != layerStack_.begin();) {
			(*--iterator)->OnEvent(event);
			if (event.IsHandled()) break;
		}
	}

	void Application::Run() {
		while (running_) {
			pWindow_->OnUpdate();
			for (Layer* layer : layerStack_) {
				layer->OnUpdate();
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		running_ = false;
		return true;
	}
}