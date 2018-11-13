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
vec3 lightColor = vec3(1.0,0.4,0.0);

vec3 objectColor = vec3 (1.0, 0.7, 0.5);

void main(){
    // Ambient lighting
    vec3 ambient = light.ambient * lightColor * matrl.ambient;
//
    vec3 norm = normalize(normal);
    vec3 lightdir = normalize(LightPosition - fragPos);
//    // Diffuse lighting
//    vec3 diffuse = light.diffuse * lightColor * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
    vec3 viewDir = normalize(eye_pos - fragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    // Specular lighting
    vec3 specular = light.specular * lightColor * (3 * spec * matrl.specular);
	gl_FragColor = vec4 (ambient+specular, 0.2);

}