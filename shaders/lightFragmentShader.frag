#version 330

in vec3 fragPos;
in vec2 texCoord;

uniform vec3 eye_pos;
uniform vec3 lightColor;

vec3 norm;

void main(){
	gl_FragColor = vec4 (lightColor, 1.0);
}
