#shader #vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;   //VertexPositions in lab5-6 shader
out vec3 Normal;	//These three are sent further down the
out vec2 TexCoords; //pipeline to the fragment shader

uniform mat4 u_TransformationMat = mat4(1); //Our uniforms for passing in data
uniform mat4 u_ViewMat		     = mat4(1);
uniform mat4 u_ProjectionMat     = mat4(1);

void main()
{
	FragPos   = vec3(u_TransformationMat * vec4(aPos, 1.0));
	Normal    = mat3(transpose(inverse(u_TransformationMat))) * aNormal;
	TexCoords = aTexCoords;

	gl_Position = u_ProjectionMat * u_ViewMat * vec4(FragPos, 1.0);
}

#shader #fragment
#version 430 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {  //For directional lights
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight { //Struct for point lights
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//If more than one point light use #define NR_POINT_LIGHTS as a constant
//#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;

//Function prototypes
vec3 calcDirLight(Dirlight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//Function implementations

//Calculates the color when using a point light


void main()
{
	//Properties
	vec3 norm    = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    /*vec3 result = CalcDirLight(dirLight, norm, viewDir);*/
    // phase 2: point lights
	vec3 result = calcPointLight(pointLight, norm, FragPos, viewDir);    
    // phase 3: spot light 
	//USE += if using more than one light
    vec3 result += calcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

//Calculates the color when using a directional light
vec3 calcDirLight(Dirlight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse light
	float diff = max(dot(normal, lightDir), 0.0);
	//Specular light
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//Combine results for Phong shading
	vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	//Diffuse light
	float diff = max(dot(normal, lightDir), 0.0);
	//Specular light
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//Combine results for Phong model
	vec3 ambient  = light.ambient  *        vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}