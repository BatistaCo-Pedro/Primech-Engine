#include "ppch.h"
#include "WindowsInput.h"

#include "Primech/Core/Application.h"
#include <GLFW/glfw3.h>

namespace PriMech {
	//Define the static instance to a new Windows Input, used in Input.h, where method returns based on Instance
	Scope<Input> Input::inputInstance_ = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keycode) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT; //return true if a key was or is being pressed
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS; //return true if a button was pressed
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return std::pair<float, float>((float)xPos, (float)yPos);
	}

	float WindowsInput::GetMouseXImpl() {
		auto [xPos, yPos] = GetMousePosImpl();
		return xPos;
	}

	float WindowsInput::GetMouseYImpl() {
		auto [xPos, yPos] = GetMousePosImpl();
		return yPos;
	}
}
