#pragma once
#include "Primech/Renderer/Buffer.h"
#include <memory>

namespace PriMech {
	class VertexArray {
	public:
		virtual ~VertexArray() {}

		static VertexArray* Create();
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
	};
}