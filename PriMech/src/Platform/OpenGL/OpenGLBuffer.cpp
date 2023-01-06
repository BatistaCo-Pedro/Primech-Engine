#include "ppch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace PriMech {
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		PM_PROFILE_FUNCTION();
		//creating OpenGL Buffer ans assigning Data to it
		glCreateBuffers(1, &rendererID_);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); //no data
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		PM_PROFILE_FUNCTION();
		//creating OpenGL Buffer ans assigning Data to it
		glCreateBuffers(1, &rendererID_);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const{
		PM_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
	}

	void OpenGLVertexBuffer::UnBind() const {
		PM_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		PM_PROFILE_FUNCTION();
		glDeleteBuffers(1, &rendererID_);
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :  count_(count){
		PM_PROFILE_FUNCTION();
		//creating OpenGL Buffer ans assigning Data to it
		glCreateBuffers(1, &rendererID_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind() const {
		PM_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
	}

	void OpenGLIndexBuffer::UnBind() const {
		PM_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		PM_PROFILE_FUNCTION();
		glDeleteBuffers(1, &rendererID_);
	}
}