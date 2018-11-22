#version 330

layout (location=0) in vec3 vertex_position;

out vec3 texCoord;

uniform mat4 view;
uniform mat4 proj;

void main() {
    texCoord = vertex_position;
    gl_Position = proj * mat4(mat3(view))* vec4(vertex_position, 1.0);
}
