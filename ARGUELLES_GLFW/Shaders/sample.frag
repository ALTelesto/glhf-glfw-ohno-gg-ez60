#version 330 core

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

uniform float ambientStr;
uniform vec3 ambientColor;

//uniform float attenuation;

in vec2 texCoord;

in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor;

void main(){
	
	vec3 normal = normalize(normCoord);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal,lightDir),0.0);

	vec3 diffuse = diff * lightColor;
	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	float distance = length(lightPos - fragPos);
	float attenuation = 10.0 / (1.0 + 0.22*distance + 0.20*(distance*distance));

	//				 r    g    b    a
	//FragColor = vec4(0.0, 0.7, 0.0, 1.0);
	//FragColor = texture(tex0, texCoord);
	FragColor = vec4(specColor*attenuation + diffuse*attenuation + ambientCol*attenuation,1.0) * texture(tex0, texCoord);
}