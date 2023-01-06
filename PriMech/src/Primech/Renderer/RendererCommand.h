#pragma once

#include "RendererAPI.h"

namespace PriMech {
	class RendererCommand {
	public:
		inline static void Init() {
			s_RendererAPI_->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI_->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI_->SetClearColor(color);
		};
		inline static void Clear() {
			s_RendererAPI_->Clear();
		};
		inline static void ClearWithColor(const glm::vec4& color) {
			s_RendererAPI_->ClearWithColor(color);
		};

		inline static void DrawIndexed(const Ref<VertexArray> vertexArray, uint32_t indexCount = 0) {
			s_RendererAPI_->DrawIndexed(vertexArray, indexCount);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI_;
	};
}