#pragma once

#include "RendererCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace PriMech {
	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera); //TODO add camera, environment etc.. as params
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrixData_;
		};

		static SceneData* sceneData_;
	};
}
