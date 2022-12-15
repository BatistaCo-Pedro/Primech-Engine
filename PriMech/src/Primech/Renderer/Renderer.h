#pragma once

#include "RendererCommand.h"

namespace PriMech {
	class Renderer {
	public:
		static void BeginScene(); //TODO add camera, environment etc.. as params
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
