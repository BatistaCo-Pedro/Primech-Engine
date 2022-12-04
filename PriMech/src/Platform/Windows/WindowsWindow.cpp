#include "ppch.h"
#include "Primech/Log.h"
#include "WindowsWindow.h"

namespace PriMech{
	static bool s_GLFWInitialized = false;

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

			s_GLFWInitialized = true;
		}

		window_ = glfwCreateWindow((int)props.width, (int)props.height, wData_.title_.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window_);
		glfwSetWindowUserPointer(window_, &wData_);
		SetVSync(true);
	}

	void WindowsWindow::ShutDown() {
		glfwDestroyWindow(window_);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window_);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		wData_.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return wData_.VSync; }
}