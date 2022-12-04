#include "ppch.h"
#include "Primech/Log.h"
#include "WindowsWindow.h"
#include <Primech/Events/ApplicationEvent.h>
#include <Primech/Events/KeyEvent.h>
#include <Primech/Events/MouseEvent.h>

namespace PriMech{
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

	void WindowsWindow::Init(const WindowProps& props) {
		wData_.title_ = props.title;
		wData_.height_ = props.height;
		wData_.width_ = props.width;

		PM_CORE_DEBUG("Craeting Window {0} ({1}, {2})", props.title, props.width, props.height);
	
		if (!s_GLFWInitialized) {
			//TODO terminate glfw on system shutdown
			int success = glfwInit();
			PM_CORE_ASSERT(scucces, "Could not initalize GLFW!");
			glfwSetErrorCallback(glfwErrorCallback);

			s_GLFWInitialized = true;
		}

		window_ = glfwCreateWindow((int)props.width, (int)props.height, wData_.title_.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window_);
		glfwSetWindowUserPointer(window_, &wData_);
		SetVSync(true);

		//Set GLFW callbacks , Lambda function --> []()
		glfwSetWindowSizeCallback(window_, [](GLFWwindow* glfwWindow, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			WindowResizeEvent event(width, height);
			data.width_ = width;
			data.height_ = height;
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(window_, [](GLFWwindow* glfwWindow) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(window_, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			
			switch (action)
			{
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window_, [](GLFWwindow* glfwWindow, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			
			switch (action)
			{
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window_, [](GLFWwindow* glfwWindow, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(window_, [](GLFWwindow* glfwWindow, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(glfwWindow);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::ShutDown() {
		glfwDestroyWindow(window_);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window_);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		wData_.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return wData_.VSync; }
}