#version 330 core

uniform sampler2D tex0;

in vec2 texCoord;

out vec4 FragColor;

void main(){
	//				 r    g    b    a
	//FragColor = vec4(0.0, 0.7, 0.0, 1.0);

	FragColor = texture(tex0, texCoord);
}