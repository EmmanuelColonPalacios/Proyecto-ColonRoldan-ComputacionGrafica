#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
uniform sampler2D texture1;

struct Material {
    vec3 specular; // O vec3 specular para iluminacion plana
    float shininess;
};
struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in float trans;

out vec4 color;
uniform vec4 colorAlpha;

void main()
{
 
    vec4 texColor= texture(texture1,TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;


}