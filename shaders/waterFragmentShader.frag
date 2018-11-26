#version 330

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 fragPos;
in vec2 texCoord;
in mat3 TBN;

uniform sampler2D tex;
uniform sampler2D normalMap;
uniform samplerCube skymap;

uniform vec3 eye_pos;
uniform material matrl;
#define nrPointLights 2
uniform DirLight light;
uniform PointLight pLights[nrPointLights];


vec3 norm;
vec3 calcDirLight(DirLight l);
vec3 calcPointLight(PointLight pl);

void main(){
    norm = texture(normalMap, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 view = normalize(fragPos - eye_pos);
    vec3 R = reflect(view, norm);

    vec3 lightResult = calcDirLight(light);
    for (int i=0; i< nrPointLights; i++) {
        lightResult += calcPointLight(pLights[i]);
    }


	gl_FragColor = mix(texture(skymap,R),texture(tex,texCoord)*vec4 (lightResult, 1.0),0.4);
}

vec3 calcDirLight(DirLight l) {
    // Ambient lighting
    vec3 ambient = l.ambient * matrl.ambient;
    vec3 lightdir = normalize(-l.direction);
    // Diffuse lighting
    vec3 diffuse = l.diffuse * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
    vec3 viewDir = normalize(eye_pos - fragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matrl.shininess);
    // Specular lighting
    vec3 specular = l.specular * (spec * matrl.specular);
    return ambient+diffuse+specular;
}

vec3 calcPointLight(PointLight pl) {
    // Attenuation factor
    float distance = length(pl.position - fragPos);
    float attenuation = 1.0 / (pl.constant + pl.linear * distance +
                         pl.quadratic * (distance * distance));
    // Ambient lighting
    vec3 ambient = pl.ambient * matrl.ambient;
    vec3 lightdir = normalize(pl.position - fragPos);
    // Diffuse lighting
    vec3 diffuse = pl.diffuse * (max(dot(lightdir,norm), 0.0) * matrl.diffuse);
    vec3 viewDir = normalize(eye_pos - fragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matrl.shininess);
    // Specular lighting
    vec3 specular = pl.specular * (spec * matrl.specular);
    return attenuation*(ambient+diffuse+specular);

}