#type vertex
#version 330 core
			
layout(location = 0) in vec3 attributePosition;
layout(location = 1) in vec2 attributeTextureCoord;
			
uniform mat4 uniformViewProjection;
uniform mat4 uniformTransform;

out vec2 varTextureCoord;

void main() {
	varTextureCoord = attributeTextureCoord;
	gl_Position = uniformViewProjection * uniformTransform * vec4(attributePosition, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 outColor;

in vec2 varTextureCoord;
			
uniform sampler2D uniformTexture;

void main() {
	outColor = texture(uniformTexture, varTextureCoord * 10.0) * vec4(1.0, 0.8, 0.8, 1.0);
}