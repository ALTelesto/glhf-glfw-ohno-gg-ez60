#version 330 core

//texture
uniform sampler2D tex0;

//point light
uniform vec3 plightPos;
uniform vec3 plightColor;
uniform float pIntensity;

//directional light
uniform vec3 dlightPos;
uniform vec3 dlightColor;
uniform float dIntensity;

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
	
	//point light
	vec3 normal = normalize(normCoord);

	vec3 plightDir = normalize(plightPos - fragPos);
	float pdiff = max(dot(normal,plightDir),0.0);
	//vec3 pdiffuse = pdiff * glm::vec3(0.f,1.f,0.f);
	vec3 pdiffuse = pdiff * plightColor;

	//directional light
	float ddiff = max(dot(normal, -dlightPos),0.0);
	vec3 ddiffuse = ddiff * dlightColor;

	//ambient calcu
	vec3 ambientCol = ambientColor * ambientStr;	//ambient light

	//specular direction
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 preflectDir = reflect(-plightDir, normal);
	vec3 dreflectDir = reflect(dlightPos, normal);
	
	//specular calcu
	float pspec = pow(max(dot(preflectDir, viewDir), 0.1), specPhong);
	float dspec = pow(max(dot(dreflectDir, viewDir), 0.1), specPhong);
	vec3 pspecColor = pspec * specStr * plightColor;
	vec3 dspecColor = dspec * specStr * dlightColor;
	
	//---- point light stuff here ----
	//get the distance between the fragment and the light
	float distance = length(plightPos - fragPos);
	//inverse square law
	//get smooth attenuation wherein light intensity decrease with increasing distance from source
	//source: https://learnopengl.com/Lighting/Light-casters
	float attenuation = pIntensity / (1.0 + 0.22*distance + 0.20*(distance*distance));

	vec3 pointlight = (pdiffuse + pspecColor) * attenuation;
	vec3 dirlight = ddiffuse + dspecColor;

	//				    r    g    b    a
	//FragColor = vec4(0.0, 0.7, 0.0, 1.0);		//solid color
	//FragColor = texture(tex0, texCoord);		//textured

	//application of texture color + all lights
	vec3 combinedLights = pointlight + (dirlight * dIntensity) + ambientCol;

	FragColor = vec4(combinedLights,1.0) * texture(tex0, texCoord);	//shaded texture
}