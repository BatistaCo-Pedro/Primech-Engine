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
		pWindow_ = std::unique_ptr<Window>(Window::Create(WindowProps("PriMech Engine",1920, 1080)));
		//Bind the Application defined OnEvent Method to the callback var of Window
		//Theres no suitable conversion from OnEvent() to std::function<void(Event&)> so we bind the functions
		pWindow_->SetEventCallback(PM_BIND_EVENT_FUNCTION(Application::OnEvent));
		//Call the Logger; Logging macros are defined in Log.h
		PM_CORE_INFO("CONSTUCTOR CALLED FOR APPLICATION");

		imGuiLayer_ = new ImGuiLayer();
		PushOverlay(imGuiLayer_);

		glGenVertexArrays(1, &vertexArray_);
		glBindVertexArray(vertexArray_);

		//defining the points pos
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};

		uint32_t indices[3] = { 0, 1, 2 };

		vertexBuffer_.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		
		indexBuffer_.reset(IndexBuffer::Create(indices, (sizeof(indices) / sizeof(vertices[0]))));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attributePosition;

			out vec3 varPosition;

			void main() {
				varPosition = attributePosition;
				gl_Position = vec4(attributePosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 outColor;

			in vec3 varPosition;

			void main() {
				vec4 color = vec4(varPosition * 0.5 + 0.5, 1.0);
				outColor = color;
			}
		)";

		shader_.reset(new Shader(vertexSrc, fragmentSrc));
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
		//Binding once agian becuase no suitable conversion
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
			glClearColor(0.1, 0.1, 0.1, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			shader_->Bind();

			glBindVertexArray(vertexArray_);
			glDrawElements(GL_TRIANGLES, indexBuffer_->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : layerStack_) {
				layer->OnUpdate();
			}

			imGuiLayer_->Begin();
			for (Layer* layer : layerStack_) {
				layer->OnImGuiRender();
			}
			imGuiLayer_->End();

			pWindow_->OnUpdate(); //updates the frame
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		running_ = false;
		return true;
	}
}