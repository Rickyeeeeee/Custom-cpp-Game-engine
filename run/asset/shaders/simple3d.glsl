#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec3 v_FragPos;
out vec4 v_Color;
out vec3 v_Normal;

void main()
{
    vec4 pos = u_Model * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * pos;
    v_FragPos = vec3(pos.x, pos.y, pos.z);
    v_Color = a_Color;
    v_Normal = a_Normal;
}

#type pixel
#version 330 core
struct PointLight
{
    float ambient;
    float diffuse;
    float specular;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
    vec3 position;
};

struct DirectionalLight
{
    vec3 direction;

    float ambient;
    float diffuse;
    float specular;

    vec3 color;
};

layout(location = 0) out vec4 FragColor;

in vec4 v_Color;
in vec3 v_Normal;
in vec3 v_FragPos;  

uniform vec3 u_ViewPos;

uniform PointLight[4] u_PointLights;  
uniform int u_PointLightSize;
uniform DirectionalLight u_DirLight; 
uniform int u_HasDirectionalLight;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(v_Color.x, v_Color.y, v_Color.z);
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    if (u_HasDirectionalLight > 0)
        result = CalcDirectionalLight(u_DirLight, norm, viewDir);
    for (int i = 0; i < u_PointLightSize; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    // result = vec3(1.0f, 1.0f, 1.0f);
    FragColor = vec4(result * objectColor, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
     // ambient
    float ambient = light.ambient;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float diffuse = light.diffuse * diff;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = light.specular * spec;  

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));    

    return (ambient + diffuse + specular) * attenuation * light.color;

}
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float shiness = 3;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
 
    float ambient = light.ambient;
    float diffuse = light.diffuse * diff;
    float specular = light.specular * spec;

    return (ambient + diffuse + specular) * light.color;
}
