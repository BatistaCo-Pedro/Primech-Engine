#include "ppch.h"
#include "Renderer.h"

#include "Primech/Renderer/camera/OrthographicCamera.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"
#include "Primech/Renderer/Renderer2d.h"

#include <glad/glad.h>

namespace PriMech {
	Scope<Renderer::SceneData> Renderer::sceneData_ = CreateScope<Renderer::SceneData>();

	void Renderer::Init() {
		RendererCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RendererCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		sceneData_->viewProjectionMatrixData_ = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(sceneData_->viewProjectionMatrixData_, "uniformViewProjection");
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(transform, "uniformTransform");
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}