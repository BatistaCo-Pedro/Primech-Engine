#include "ppch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace PriMech {
	
	Application::Application() {

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
	}
}