#pragma once

#include "Primech/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace PriMech {
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* windowHandle_;
	};
}