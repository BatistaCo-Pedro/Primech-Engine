#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

namespace PriMech {
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minBound, const glm::vec2& maxBound);

		const Ref<Texture2D> GetTexture() { return texture_; }
		glm::vec2* GetTextureCoords() { return textureCoords; }	

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);
	private:
		Ref<Texture2D> texture_;

		glm::vec2 textureCoords[4];
	};
}

