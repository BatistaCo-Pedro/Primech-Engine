#type vertex
#version 330 core
			
layout(location = 0) in vec3 attributePosition;
layout(location = 1) in vec4 attributeColor;
layout(location = 2) in vec2 attributeTextureCoord;
			
uniform mat4 uniformViewProjection;

out vec2 varTextureCoord;
out vec4 varColor;

void main() {
	varColor = attributeColor;
	varTextureCoord = attributeTextureCoord;
	gl_Position = uniformViewProjection * vec4(attributePosition, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 outColor;

in vec4 varColor;
in vec2 varTextureCoord;
		
uniform vec4 uniformColor;
uniform float uniformTileMultiplier;
uniform sampler2D uniformTexture;

void main() {
	// outColor = texture(uniformTexture, varTextureCoord * uniformTileMultiplier) * uniformColor;
	outColor = varColor;
}