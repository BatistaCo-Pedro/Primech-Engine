#include "ppch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"

namespace PriMech {
	
#define BIND_EVENT_FUNCTION(x)	std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		pWindow_ = std::unique_ptr<Window>(Window::Create());
		pWindow_->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
	}

	Application::~Application() {}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));

		//Printign all events for debugging purposes
		if (event.IsInCategory(EventCategoryApplication) ||
			event.IsInCategory(EventCategoryKeyboard)) {
			PM_CORE_TRACE(event); //keyboard and window events in white
		}
		else {
			PM_CORE_DEBUG(event); //mouse events in blue
		}
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 900);
		if (e.IsInCategory(EventCategoryApplication)) {
			PM_INFO(e); //log event
		}
		if (e.IsInCategory(EventCategoryInput)) {
			PM_WARN(e);
		}

		while (running_) {
			pWindow_->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		running_ = false;
		return true;
	}
}