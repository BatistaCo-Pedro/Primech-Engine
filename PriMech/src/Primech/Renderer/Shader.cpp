#include "ppch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"

namespace PriMech {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return new OpenGLShader(vertexSrc, fragmentSrc);
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
		return nullptr;
	}
}