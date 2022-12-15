#include "ppch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace PriMech {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		PM_CORE_ASSERT(false, "Unkown ShaderDataType at DataTypeConversion()");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &rendererID_);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &rendererID_);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(rendererID_);
	}

	void OpenGLVertexArray::UnBind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(rendererID_);
		vertexBuffer->Bind();

		PM_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		uint32_t index = 0;
		const BufferLayout layout = vertexBuffer->GetLayout();
		for (const BufferElement element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
			index++;
		}
		vertexBuffers_.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		glBindVertexArray(rendererID_);
		indexBuffer->Bind();

		indexBuffer_ = indexBuffer;
	}

	VertexArray* OpenGLVertexArray::Create() {
	
		return nullptr;
	}
}