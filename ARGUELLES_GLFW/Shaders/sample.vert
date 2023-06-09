#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTex;

uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;

out vec2 texCoord;
	
void main(){
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	texCoord = aTex;
}