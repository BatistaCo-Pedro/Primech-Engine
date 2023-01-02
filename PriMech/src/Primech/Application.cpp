#include "ppch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"

#include "Primech/Renderer/Renderer.h"
#include "GLFW/glfw3.h"

#include <Primech/Input.h>

namespace PriMech {

	Application* Application::instance_ = nullptr; //init Pointer 

	//This cosntructor gets called whena  new Application is externally initalized with CreateApplication()
	Application::Application() {
		PM_CORE_ASSERT(!instance_, "Application already exists")
		instance_ = this; //Set App Instance Pointer to pint to this App instance

		//create Window as graphical interface, event callback funcitons are defined in the Window class		
		windowPtr_ = Scope<Window>(Window::Create(WindowProps("PriMech Engine", 1920, 1080)));

		//Bind the Application defined OnEvent Method to the callback var of Window
		//Theres no conversion from OnEvent() to std::function<void(Event&)> so we bind the functions
		windowPtr_->SetEventCallback(PM_BIND_EVENT_FUNCTION(Application::OnEvent));

		Renderer::Init();

		//Call the Logger; Logging macros are defined in Log.h
		PM_CORE_INFO("CONSTUCTOR CALLED FOR APPLICATION");

		imGuiLayer_ = new ImGuiLayer();
		PushOverlay(imGuiLayer_);
	}

	Application::~Application() {}

	//Push layer to the starting side of the stack
	void Application::PushLayer(Layer* layer) {
		layerStack_.PushLayer(layer);
	}

	//Push layer to the ending side of the stack
	void Application::PushOverlay(Layer* overlay) {
		layerStack_.PushOverlay(overlay);
	}

	//This Method is binded to Window callback and is called when an event occurs
	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(PM_BIND_EVENT_FUNCTION(Application::OnWindowClose));

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
			float time = (float)glfwGetTime(); //glfw is temp
			Timestep timestep = time - lastFrameTime_;
			lastFrameTime_ = time;
		

			//Call OnUpdate() for every layer in the stack
			for (Layer* layer : layerStack_) {
				layer->OnUpdate(timestep);
			}

			imGuiLayer_->Begin();
			for (Layer* layer : layerStack_) {
				layer->OnImGuiRender();
			}
			imGuiLayer_->End();

			windowPtr_->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		running_ = false;
		return true;
	}
}