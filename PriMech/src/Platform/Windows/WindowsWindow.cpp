#include "ppch.h"
#include "Primech/Core/Logging/Log.h"
#include "WindowsWindow.h"
#include <Primech/Events/ApplicationEvent.h>
#include <Primech/Events/KeyEvent.h>
#include <Primech/Events/MouseEvent.h>

#include <Platform/OpenGL/OpenGLContext.h>

namespace PriMech { 
	static bool s_GLFWInitialized = false;

	static void glfwErrorCallback(int error, const char* description) {
		PM_CORE_ERROR("GLFW error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}
	WindowsWindow::~WindowsWindow() {}

	//Gets called by the constructor on object initalization
	void WindowsWindow::Init(const WindowProps& props) {
		wData_.title_ = props.title;
		wData_.height_ = props.height;
		wData_.width_ = props.width;

		PM_CORE_DEBUG("Creating Window (Window props): {0} ({1}, {2})", props.title, props.width, props.height);
		PM_CORE_DEBUG("Creating Window (Window Data): {0} ({1}, {2})", wData_.title_, wData_.width_, wData_.height_);
	
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			PM_CORE_ASSERT(success, "Could not initalize GLFW!");
			//set callback function to be called in case of error
			glfwSetErrorCallback(glfwErrorCallback);

			s_GLFWInitialized = true;
		}

		//create the actual window and save it to a variable of type GLFWwindow
		window_ = glfwCreateWindow(wData_.width_, wData_.height_, wData_.title_.c_str(), nullptr, nullptr);
		context_ = new OpenGLContext(window_);

		context_->Init();

		//set the Window Pointer to the WindowsWindow::WindowData struct
		glfwSetWindowUserPointer(window_, &wData_);
		SetVSync(true); //cap frames/sec to monitors refresh rate (VSync)

		//Set GLFW callbacks , Lambda function --> []()
		glfwSetWindowSizeCallback(window_, [](GLFWwindow* glfwWindow, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			WindowResizeEvent event(width, height);
			data.width_ = width;
			data.height_ = height;
			PM_CORE_TRACE("{0}, {1}", width, height);
			data.EventCallback(event); //This calls wData_.Eventcallback which is binded to Application OnEvent()
		});

		glfwSetWindowCloseCallback(window_, [](GLFWwindow* glfwWindow) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			WindowCloseEvent event;
			data.EventCallback(event); //This calls wData_.Eventcallback which is binded to Application OnEvent()
		});

		glfwSetKeyCallback(window_, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			
			switch (action)
			{
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event); //This calls wData_.Eventcallback which is binded to Application OnEvent()
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event); //This calls wData_.Eventcallback which is binded to Application OnEvent()
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event); //This calls wData_.Eventcallback which is binded to Application OnEvent()
					break;
				}
			}
		});

		glfwSetCharCallback(window_, [](GLFWwindow* glfwWindow, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(window_, [](GLFWwindow* glfwWindow, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			
			switch (action)
			{
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event); //This calls wData_.Eventcallback whcih is binded to Application OnEvent()
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event); //This calls wData_.Eventcallback whcih is binded to Application OnEvent()
					break;
				}
			}
		});

		glfwSetScrollCallback(window_, [](GLFWwindow* glfwWindow, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event); //This calls wData_.Eventcallback whcih is binded to Application OnEvent()
		});

		glfwSetCursorPosCallback(window_, [](GLFWwindow* glfwWindow, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event); //This calls wData_.Eventcallback whcih is binded to Application OnEvent()
		});
	}

	void WindowsWindow::ShutDown() {
		glfwDestroyWindow(window_);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents(); //check if theres is any pending event and handles it
		context_->SwapBuffers(); //updates the frame
	}

	void WindowsWindow::SetVSync(bool enabled) {
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		wData_.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return wData_.VSync; }
}