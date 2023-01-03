#include "ppch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace PriMech {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle_(windowHandle) {
		PM_CORE_ASSERT(windowHandle_, "Window handle doesnt exist")
	}

	OpenGLContext::~OpenGLContext() {}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(windowHandle_);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PM_CORE_ASSERT(status, "Failed to initialize glad")

		PM_CORE_INFO("Opengl Vendor: {0}", (char*)glGetString(GL_VENDOR));
		PM_CORE_INFO("Opengl Renderer: {0}", (char*)glGetString(GL_RENDERER));
		PM_CORE_INFO("Opengl Version: {0}", (char*)glGetString(GL_VERSION));

		#ifdef PM_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				PM_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers() {
		//Update Frame by swapping the buffer 
		glfwSwapBuffers(windowHandle_);
	}
}