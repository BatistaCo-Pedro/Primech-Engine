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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual const VertexBuffer& GetVertexBuffer() const = 0;
	};
}