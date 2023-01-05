#pragma once

#include "PriMech/Renderer/RendererCommand.h"
#include "PriMech/Renderer/camera/OrthographicCamera.h"
#include "PriMech/Renderer/Shader.h"

namespace PriMech {
	class Renderer {
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera); //TODO add camera, environment etc.. as params
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrixData_;
		};

		static Scope<SceneData> sceneData_;
	};
}
