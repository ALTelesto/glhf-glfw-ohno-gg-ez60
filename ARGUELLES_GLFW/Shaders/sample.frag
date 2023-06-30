#version 330 core

//texture
uniform sampler2D tex0;

//light
uniform vec3 lightPos;
uniform vec3 lightColor;

//specular
uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

//ambient light
uniform float ambientStr;
uniform vec3 ambientColor;

in vec2 texCoord;

in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor;

void main(){
	
	//light calcu
	vec3 normal = normalize(normCoord);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal,lightDir),0.0);

	//ambient calcu
	vec3 diffuse = diff * lightColor;				//
	vec3 ambientCol = ambientColor * ambientStr;	//ambient light

	//specular direction
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	//specular calcu
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;
	
	//---- point light stuff here ----
	//get the distance between the fragment and the light
	float distance = length(lightPos - fragPos);
	//inverse square law
	//get smooth attenuation wherein light intensity decrease with increasing distance from source
	//source: https://learnopengl.com/Lighting/Light-casters
	float attenuation = 10.0 / (1.0 + 0.22*distance + 0.20*(distance*distance));

	//				    r    g    b    a
	//FragColor = vec4(0.0, 0.7, 0.0, 1.0);		//solid color
	//FragColor = texture(tex0, texCoord);		//textured

	//application of texture color + all lights
	FragColor = vec4(specColor*attenuation + diffuse*attenuation + ambientCol*attenuation,1.0) * texture(tex0, texCoord);	//shaded texture
}