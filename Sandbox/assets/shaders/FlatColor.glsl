//Flat Color Shader
#type vertex
#version 330 core
			
layout(location = 0) in vec3 attributePosition;		

uniform mat4 uniformViewProjection;
uniform mat4 uniformTransform;

void main() {
	gl_Position = uniformViewProjection * uniformTransform * vec4(attributePosition, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 outColor;

uniform vec4 uniformColor;
			
void main() {
	outColor = uniformColor;
}