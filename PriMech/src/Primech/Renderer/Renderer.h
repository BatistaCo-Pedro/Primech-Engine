#pragma once

#include "RendererCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace PriMech {
	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera); //TODO add camera, environment etc.. as params
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrixData_;
		};

		static SceneData* sceneData_;
	};
}
