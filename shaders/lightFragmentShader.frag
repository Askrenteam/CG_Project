#version 330

in vec3 fragPos;
in vec2 texCoord;

uniform sampler2D tex;

uniform vec3 eye_pos;
uniform vec3 lightColor;

vec3 norm;

void main(){
	gl_FragColor = texture(tex,texCoord)*vec4 (lightColor, 1.0);

}
