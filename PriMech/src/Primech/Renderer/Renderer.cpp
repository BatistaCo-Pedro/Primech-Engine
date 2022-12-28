#include "ppch.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "Primech/Renderer/OrthographicCamera.h"

namespace PriMech {
	Renderer::SceneData* Renderer::sceneData_ = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera) {
		sceneData_->viewProjectionMatrixData_ = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform) {
		shader->Bind();
		shader->UploadUniformMat4(sceneData_->viewProjectionMatrixData_);
		shader->UploadUniformMat4(transform, "uniformTransform");
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}