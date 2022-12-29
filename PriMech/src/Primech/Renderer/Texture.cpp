#include "ppch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/Shader/OpenGlTexture.h"

namespace PriMech {
	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
		return nullptr;
	}
}