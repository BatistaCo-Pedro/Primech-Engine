#include "ppch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"

#include <glad/glad.h>

#include <Primech/Input.h>

namespace PriMech {

	Application* Application::instance_ = nullptr; //init Pointer 

	//This cosntructor gets called whena  new Application is externally initalized with CreateApplication()
	Application::Application() {
		PM_CORE_ASSERT(!instance_, "Application already exists")
		instance_ = this; //Set App Instance Pointer to pint to this App instance
		//create Window as graphical interface, event callback funcitons are defined in the Window class
		pWindow_ = std::unique_ptr<Window>(Window::Create());
		//Bind the Application defined OnEvent Method to the callback var of Window
		//Theres no suitable conversion from OnEvent() to std::function<void(Event&)> so we bind the functions
		pWindow_->SetEventCallback(PM_BIND_EVENT_FUNCTION(Application::OnEvent));
		//Call the Logger; Logging macros are defined in Log.h
		PM_CORE_INFO("CONSTUCTOR CALLED FOR APPLICATION");

		unsigned int id;
		glGenVertexArrays(1, &id);
	}

	Application::~Application() {}

	//Push layer to the starting side of the stack
	void Application::PushLayer(Layer* layer) {
		layerStack_.PushLayer(layer);
		layer->OnAttach();
	}

	//Push layer to the ending side of the stack
	void Application::PushOverlay(Layer* overlay) {
		layerStack_.PushOverlay(overlay);
		overlay->OnAttach();
	}

	//This Method is binded to Window callback and is called when an event occurs
	void Application::OnEvent(Event& event) {

		EventDispatcher dispatcher(event);
		//Binding once agian becuase no suitable conversion
		dispatcher.Dispatch<WindowCloseEvent>(PM_BIND_EVENT_FUNCTION(Application::OnWindowClose));

		//Logging all events for debugging purposes
		if (event.IsInCategory(EventCategoryApplication) ||
			event.IsInCategory(EventCategoryKeyboard)) {
			PM_CORE_TRACE(event); //keyboard and window events in white
		}
		else {
			PM_CORE_DEBUG(event); //mouse events in blue
		}

		//handle events on the layer
		//reverse iterating the layer stack
		for (auto iterator = layerStack_.end(); iterator != layerStack_.begin();) {
			//calls OnEvent() from Application e.g. Sandbox
			(*--iterator)->OnEvent(event);
			if (event.IsHandled()) break;
		}
	}

	//Method called by Application to start running the program
	void Application::Run() {
		while (running_) {
			pWindow_->OnUpdate(); //updates the frame
			for (Layer* layer : layerStack_) {
				layer->OnUpdate();
			}

			auto [xPos, yPos] = Input::GetMousePos();
			PM_CORE_TRACE("{0}, {1}", xPos, yPos);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		running_ = false;
		return true;
	}
}