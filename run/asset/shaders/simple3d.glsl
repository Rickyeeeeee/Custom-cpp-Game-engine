#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
// layout(location = 2) in int a_ID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_NormalModel;
uniform vec4 u_Color;
uniform mat4 u_LightSpaceMatrix;

out vec3 v_FragPos;
out vec4 v_Color;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec4 v_FragPosLightSpace;


void main()
{
    vec4 pos = u_Model * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * pos;
    v_FragPos = vec3(pos.x, pos.y, pos.z);
    v_Color = u_Color;
    v_Normal = vec3(u_NormalModel * vec4(a_Normal, 1.0f));
    v_TexCoord = a_TexCoord;
    v_FragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragPos, 1.0);
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
layout(location = 1) out int id;

in vec4 v_Color;
in vec3 v_Normal;
in vec3 v_FragPos;  
in vec2 v_TexCoord;
in vec4 v_FragPosLightSpace;

uniform vec3 u_ViewPos;

uniform sampler2D u_Texture;
uniform sampler2D u_DepthMap;

uniform PointLight[5] u_PointLights;  
uniform int u_PointLightSize;
uniform DirectionalLight u_DirLight; 
uniform int u_HasDirectionalLight;
uniform int u_id;

uniform vec2 u_Tiling;
uniform vec2 u_Offset;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float CalcShadow(vec4 fragPosLightSpace);

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(v_Color.x, v_Color.y, v_Color.z);
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    if (u_HasDirectionalLight > 0)
        result = CalcDirectionalLight(u_DirLight, norm, viewDir) * (1.0 - CalcShadow(v_FragPosLightSpace));
    int pointLightSize = u_PointLightSize;
    if (u_PointLightSize > 5)
         pointLightSize = 5;
    for (int i = 0; i < u_PointLightSize; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    // result = vec3(1.0f, 1.0f, 1.0f);
    id = u_id;
    FragColor = vec4(result * objectColor, 1.0) * texture(u_Texture, (v_TexCoord + u_Offset) * u_Tiling);
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
    float shiness = 2;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
 
    float ambient = light.ambient;
    float diffuse = light.diffuse * diff;
    float specular = light.specular * spec;

    return (ambient + diffuse + specular) * light.color;
}

float CalcShadow(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_DepthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    vec3 lightDir = normalize(u_DirLight.direction);
    // float bias = max(0.006 * (1.0 - dot(v_Normal, lightDir)), 0.005); 
    float bias = 0.005;

    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_DepthMap, 0);
    int range = 2;
    for(int x = -range; x <= range; ++x)
    {
        for(int y = -range; y <= range; ++y)
        {
            float pcfDepth = texture(u_DepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    float sampleNum = range * 2 + 1;
    shadow /= (sampleNum * sampleNum);

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}  