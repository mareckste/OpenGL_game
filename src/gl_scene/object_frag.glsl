/*#version 150
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 FragTexCoord;

// Wordspace normal
in vec4 normal;

// The final color
out vec4 FragmentColor;

// Light direction in world coordinates
vec4 lightDirection = normalize(vec4(-0.5f,-0.5f,-0.5f,0.0f));

void main() {
  // Compute diffuse lighting
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  // Lookup the color in Texture on coordinates given by fragTexCoord and apply diffuse lighting
  FragmentColor = texture(Texture, FragTexCoord) * diffuse ;
}
*/

#version 150

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform sampler2D Texture;
uniform vec3 viewPos;
uniform PointLight pointLights[2];


in vec2 FragTexCoord;
in vec3 normal;
in vec3 fragPosition;

//in vec4 normal;


out vec4 FragmentColor;


//vec4 lightDirection = normalize(vec4(-0.5f,-0.5f,-0.5f,0.0f));
vec3 PhongLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main() {

  //float diffuse = max(dot(normal, lightDirection), 2.0f);

  vec3 result;
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 norm = normalize(normal);

    result += PhongLight(pointLights[0], norm, fragPosition, viewDir);
    result += PhongLight(pointLights[1], norm, fragPosition, viewDir);



  //FragmentColor = texture(Texture, FragTexCoord) * diffuse ;
  FragmentColor = vec4(.5f*result, 1.0f);
}

vec3 PhongLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);

    float distance = length(light.position - fragPos);
    float attenuation = 3.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(Texture, FragTexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(Texture, FragTexCoord));
    vec3 specular = light.specular * spec * vec3(texture(Texture, FragTexCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}