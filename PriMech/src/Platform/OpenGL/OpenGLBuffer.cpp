#include "ppch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace PriMech {
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		//creating OpenGL Buffer ans assigning Data to it
		glCreateBuffers(1, &rendererID_);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const{
		glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
	}

	void OpenGLVertexBuffer::UnBind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &rendererID_);
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :  count_(count){
		//creating OpenGL Buffer ans assigning Data to it
		glCreateBuffers(1, &rendererID_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
	}

	void OpenGLIndexBuffer::UnBind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &rendererID_);
	}
}