#pragma once
#include "Primech/Renderer/Texture.h"

#include <glad/glad.h>

namespace PriMech {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual void SetData(void* data, uint32_t size) override;

		inline virtual uint32_t GetWidth() const override { return width_; }
		inline virtual uint32_t GetHeight() const override { return height_; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string path_;
		uint32_t width_, height_;
		uint32_t rendererID_;
		GLenum dataFormat_, internalFormat_;
	};
}


