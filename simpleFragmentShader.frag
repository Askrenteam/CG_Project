#version 330

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec3 normal;

uniform vec3 LightPosition;
uniform vec3 eye_pos;
uniform material matrl;
uniform Light light;
vec3 lightColor = vec3(1.0,1.0,1.0);

vec3 objectColor = vec3 (0.8,0.8,0.8);
//float ambient = 0.1;
float specularStrength = 0.5;

void main(){
    // Ambient lighting
    vec3 ambient = light.ambient * objectColor * matrl.ambient;

    vec3 norm = normalize(normal);
    vec3 lightdir = normalize(LightPosition - fragPos);
    // Diffuse lighting
    vec3 diffuse = light.diffuse * objectColor * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
    vec3 viewDir = normalize(eye_pos - fragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matrl.shininess);
    // Specular lighting
    vec3 specular = light.specular * lightColor * (spec * matrl.specular);
	gl_FragColor = vec4 (ambient+diffuse+specular, 1.0);

}