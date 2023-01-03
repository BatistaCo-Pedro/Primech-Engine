#include "ppch.h"
#include "Primech/Renderer/VertexArray.h"
#include "Primech/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace PriMech {
	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexArray>();
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
		return nullptr;
	}
}