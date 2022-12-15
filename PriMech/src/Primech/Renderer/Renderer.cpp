#include "ppch.h"
#include "Renderer.h"

#include "glad/glad.h"

namespace PriMech {
	void Renderer::BeginScene() {

	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}