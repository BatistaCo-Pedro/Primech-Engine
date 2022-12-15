#include "ppch.h"
#include "Primech/Renderer/VertexArray.h"
#include "Primech/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace PriMech {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
			case RendererAPI::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
			return nullptr;
	}
}