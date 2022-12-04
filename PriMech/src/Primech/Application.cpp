#include "ppch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace PriMech {
	
	Application::Application() {
		pWindow_ = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {

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
}