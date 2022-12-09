#include "ppch.h"
#include "Primech/Log.h"
#include "WindowsWindow.h"
#include <Primech/Events/ApplicationEvent.h>
#include <Primech/Events/KeyEvent.h>
#include <Primech/Events/MouseEvent.h>

#include <glad/glad.h>

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

		PM_CORE_DEBUG("Creating Window {0} ({1}, {2})", props.title, props.width, props.height);
	
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			PM_CORE_ASSERT(scucces, "Could not initalize GLFW!");
			//set callback function to be called in case of error
			glfwSetErrorCallback(glfwErrorCallback);

			s_GLFWInitialized = true;
		}

		//create the actual window and save it to a variable of type GLFWwindow
		window_ = glfwCreateWindow((int)props.width, (int)props.height, wData_.title_.c_str(), nullptr, nullptr);

		//"focus* on the created window
		glfwMakeContextCurrent(window_);
		// Load all OpenGL functions using the glfw loader function
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PM_CORE_ASSERT(status, "Failed to initialize glad")

		//set the Window Pointer to the WindowsWindow::WindowData struct
		glfwSetWindowUserPointer(window_, &wData_);
		SetVSync(true); //cap frames/sec to monitors refresh rate (VSync)

		//Set GLFW callbacks , Lambda function --> []()
		glfwSetWindowSizeCallback(window_, [](GLFWwindow* glfwWindow, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			WindowResizeEvent event(width, height);
			data.width_ = width;
			data.height_ = height;
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
		glfwSwapBuffers(window_); //updates the frame based on SwapInterval; 1 for VSync, 0 for all
	}

	void WindowsWindow::SetVSync(bool enabled) {
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		wData_.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return wData_.VSync; }
}