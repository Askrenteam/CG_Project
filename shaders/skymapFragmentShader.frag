#version 330

in vec3 texCoord;

uniform samplerCube skymap;

void main() {
//	gl_FragColor = vec4(texCoord,1.0);
	gl_FragColor = texture(skymap,texCoord);
}