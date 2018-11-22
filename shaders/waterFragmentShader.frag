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
in vec2 texCoord;
in mat3 TBN;

uniform sampler2D tex;
uniform sampler2D normalMap;
uniform samplerCube skymap;

uniform vec3 LightPosition;
uniform vec3 eye_pos;
uniform material matrl;
uniform Light light;
vec3 lightColor = vec3(1.0,1.0,1.0);

vec3 objectColor = vec3 (0.2,0.8,0.1);

void main(){
    vec3 norm = texture(normalMap, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 view = normalize(fragPos - eye_pos);
    vec3 R = reflect(view, norm);

    // Ambient lighting
    vec3 ambient = light.ambient * matrl.ambient;
//    vec3 ambient = light.ambient * vec3(1.0,0.0,0.0) * matrl.ambient;

//    vec3 norm = normalize(normal);
    vec3 lightdir = normalize(LightPosition - fragPos);
    // Diffuse lighting
    vec3 diffuse = light.diffuse * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
//    vec3 diffuse = light.diffuse * vec3(0.0,0.0,1.0) * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
    vec3 viewDir = normalize(eye_pos - fragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matrl.shininess);
    // Specular lighting
    vec3 specular = light.specular * lightColor * (spec * matrl.specular);
//    vec3 specular = light.specular * vec3(0.0,1.0,0.0) * (spec * matrl.specular);
//    gl_FragColor = vec4(1.0,0.0,0.0,1.0);
//	gl_FragColor = vec4 (ambient+diffuse+specular, 1.0);
	gl_FragColor = mix(texture(skymap,R),texture(tex,texCoord)*vec4 (ambient+diffuse+specular, 1.0),0.4);

}