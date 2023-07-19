#version 330 core

uniform sampler2D tex0;

uniform vec4 FragColorIn;
out vec4 FragColor;

in vec2 texCoord;

in vec3 normCoord;
in vec3 fragPos;

void main(){
	//				 r    g    b    a
	//FragColor = vec4(0.0, 0.7, 0.0, 1.0);
	vec4 textureColor = texture(tex0, texCoord);

	FragColor = FragColorIn * textureColor;
}