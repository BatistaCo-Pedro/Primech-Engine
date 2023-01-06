#type vertex
#version 330 core
			
layout(location = 0) in vec3 attributePosition;
layout(location = 1) in vec4 attributeColor;
layout(location = 2) in vec2 attributeTextureCoord;
layout(location = 3) in float attributeTextureIndex;
layout(location = 4) in float attributeTilingFactor;
			
uniform mat4 uniformViewProjection;

out vec2 varTextureCoord;
out vec4 varColor;
out float varTextureIndex;
out float varTilingFactor;

void main() {
	varColor = attributeColor;
	varTextureCoord = attributeTextureCoord;
	varTextureIndex = attributeTextureIndex;
	varTilingFactor = attributeTilingFactor;
	gl_Position = uniformViewProjection * vec4(attributePosition, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 outColor;

in vec4 varColor;
in vec2 varTextureCoord;
in float varTextureIndex;
in float varTilingFactor;
	
uniform sampler2D uniformTextures[32];

void main() {
	outColor = texture(uniformTextures[int(varTextureIndex)], varTextureCoord * varTilingFactor) * varColor;
}