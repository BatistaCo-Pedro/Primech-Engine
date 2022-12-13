#pragma once

#include "Primech/Renderer/Buffer.h"

namespace PriMech {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
	private:
		uint32_t rendererID_;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		inline virtual uint32_t GetCount() const override { return count_; };
	private:
		uint32_t count_;
		uint32_t rendererID_;
	};
}