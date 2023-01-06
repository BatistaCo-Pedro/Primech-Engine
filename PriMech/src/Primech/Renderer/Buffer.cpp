#include "ppch.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Renderer.h"

namespace PriMech {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(size);
		case RendererAPI::API::None:	PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(vertices, size);
			case RendererAPI::API::None:	PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count);
			case RendererAPI::API::None:	PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified");
		return nullptr;
	}
}