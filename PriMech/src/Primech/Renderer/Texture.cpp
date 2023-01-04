#include "ppch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/Shader/OpenGlTexture.h"

namespace PriMech {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(width, height);
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
			return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
		return nullptr;
	}
}