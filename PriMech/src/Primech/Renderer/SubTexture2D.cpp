#include "ppch.h"
#include "SubTexture2D.h"

namespace PriMech {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minBound, const glm::vec2& maxBound) : texture_(texture) {
		textureCoords[0] = { minBound.x, minBound.y };
		textureCoords[1] = { maxBound.x, minBound.y };
		textureCoords[2] = { maxBound.x, maxBound.y };
		textureCoords[3] = { minBound.x, maxBound.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize) {		
		glm::vec2 minBound = { coords.x * spriteSize.x / texture->GetWidth(), (coords.y * spriteSize.y) / texture->GetHeight() }; //bottom-left
		glm::vec2 maxBound = { (coords.x + 1) * spriteSize.x / texture->GetWidth(), ((coords.y + 1) * spriteSize.y) / texture->GetHeight() }; //top-right
		
		return CreateRef<SubTexture2D>(texture, minBound, maxBound);
	}
}