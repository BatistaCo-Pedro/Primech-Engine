#include "ppch.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "Primech/Renderer/OrthographicCamera.h"
#include <Platform/OpenGL/Shader/OpenGLShader.h>

namespace PriMech {
	Renderer::SceneData* Renderer::sceneData_ = new Renderer::SceneData;

	void Renderer::Init() {
		RendererCommand::Init();
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