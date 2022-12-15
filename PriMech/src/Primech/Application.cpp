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

		vertexArray_.reset(VertexArray::Create());

		//defining the points pos
		/*
			Offset -> item1, item2, item3, item4
			Choice: item4 -> offset item1 + item2 + item3 in bits
			Choice: item2 -> offset item1 in bits
			Offset is absolut not realtive, always from the start, 0

			Stride -> item1, item2, item3
					  item1, item2, item3
			The Stride is the difference from vertex to vertex in bits

		*/
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.3f, 0.3f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.3f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
		};

		uint32_t indices[3] = { 0, 1, 2 };

		//Creating new unqiue pointer
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "attributePosition" },
			{ ShaderDataType::Float4, "attributeColor" },
		};
		vertexBuffer->SetLayout(layout);
		vertexArray_->AddVertexBuffer(vertexBuffer);

		//Creating new unqiue pointer
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, (sizeof(indices) / sizeof(indices[0]))));
		vertexArray_->SetIndexBuffer(indexBuffer);

		squareVertexArray_.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f, 
			-0.75f,  0.75f, 0.0f, 
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };	

		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "attributePosition" },
		};
		squareVertexBuffer->SetLayout(squareLayout);

		squareVertexArray_->AddVertexBuffer(squareVertexBuffer);

		std::shared_ptr<IndexBuffer> squareIndexBuffer; 
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, (sizeof(squareIndices) / sizeof(squareIndices[0]))));
		squareVertexArray_->SetIndexBuffer(squareIndexBuffer);

		//Temp
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attributePosition;
			layout(location = 1) in vec4 attributeColor;

			out vec3 varPosition;
			out vec4 varColor;

			void main() {
				varPosition = attributePosition;
				varColor = attributeColor;
				gl_Position = vec4(attributePosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 outColor;

			in vec3 varPosition;
			in vec4 varColor;

			void main() {
				vec4 color = vec4(varPosition * 0.5 + 0.5, 1.0);
				outColor = color;
				outColor = varColor;
			}
		)";



		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attributePosition;

			void main() {
				gl_Position = vec4(attributePosition, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 outColor;

			void main() {
				outColor = vec4(0.2, 0.2, 0.5, 1.0);
			}
		)";

		shader_.reset(new Shader(vertexSrc, fragmentSrc));
		blueShader_.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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

			blueShader_->Bind();
			squareVertexArray_->Bind();
			glDrawElements(GL_TRIANGLES, squareVertexArray_->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			shader_->Bind();
			vertexArray_->Bind();
			glDrawElements(GL_TRIANGLES, vertexArray_->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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