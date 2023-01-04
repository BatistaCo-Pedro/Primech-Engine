#include "ppch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace PriMech {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : width_(width), height_(height) {
		internalFormat_ = GL_RGBA8, dataFormat_ = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID_);
		glTextureStorage2D(rendererID_, 1, internalFormat_, width_, height_);

		glTextureParameteri(rendererID_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererID_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID_, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		PM_CORE_ASSERT(data, "Failed to load image!");
		width_ = width;
		height_ = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		
		internalFormat_ = internalFormat, dataFormat_ = dataFormat;

		PM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID_);
		glTextureStorage2D(rendererID_, 1, internalFormat, width_, height_);

		glTextureParameteri(rendererID_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererID_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererID_, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererID_, 0, 0, 0, width_, height_, dataFormat_, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &rendererID_);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		glTextureSubImage2D(rendererID_, 0, 0, 0, width_, height_, dataFormat_, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, rendererID_);
	}
}