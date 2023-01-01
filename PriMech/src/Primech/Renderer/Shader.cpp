#include "ppch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"

namespace PriMech {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filepath);
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
			case RendererAPI::API::None:		PM_CORE_ASSERT(false, "None RenderAPI is not supported"); return nullptr;
		}

		PM_CORE_ASSERT(false, "No RenderAPI specified")
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		PM_CORE_ASSERT(!Exists(name), "Shader already exists!");
		shaders_[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		PM_CORE_ASSERT(Exists(name), "Shader not found!");
		return shaders_[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return shaders_.find(name) != shaders_.end();
	}


}