#pragma once

#include "RendererAPI.h"

namespace PriMech {
	class RendererCommand {
	public:
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI_->SetClearColor(color);
		};
		inline static void Clear() {
			s_RendererAPI_->Clear();
		};
		inline static void ClearWithColor(const glm::vec4& color) {
			s_RendererAPI_->ClearWithColor(color);
		};

		inline static void DrawIndexed(const Ref<VertexArray> vertexArray) {
			s_RendererAPI_->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI_;
	};
}