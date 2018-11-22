#version 330

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec2 texCoord;
in mat3 TBN;

uniform sampler2D tex;
uniform sampler2D normalMap;

uniform vec3 LightPosition;
uniform vec3 eye_pos;
uniform material matrl;
uniform Light light;

vec3 calcDirLight(Light light);
vec3 norm;

void main(){
    norm = texture(normalMap, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

	gl_FragColor = texture(tex,texCoord)*vec4 (calcDirLight(light), 1.0);

}

vec3 calcDirLight(Light light) {
    // Ambient lignting
    vec3 ambient = light.ambient * matrl.ambient;
    vec3 lightdir = normalize(light.position - fragPos);
    // Diffuse lighting
    vec3 diffuse = light.diffuse * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
    vec3 viewDir = normalize(eye_pos - fragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matrl.shininess);
    // Specular lighting
    vec3 specular = light.specular * (spec * matrl.specular);
    return ambient+diffuse+specular;
}