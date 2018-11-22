#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texCoords;
in vec3 vertex_tangent;


//out vec3 LightIntensity;
out vec3 fragPos;
out vec2 texCoord;
out mat3 TBN;

//vec4 LightPosition = vec4 (10.0, 10.0, 4.0, 1.0); // Light position in world coords.
vec3 Kd = vec3 (1.0, 1.0, 1.0); // white diffuse surface reflectance
//vec3 Kd = vec3 (0.5, 0.5, 0.5); // white diffuse surface reflectance
vec3 Ld = vec3 (1.0, 1.0, 1.0); // Light source intensity



uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
uniform mat3 t_i_model; // transpose(inverse(model))

void main(){

  mat4 ModelViewMatrix = view * model;
  mat3 NormalMatrix =  mat3(ModelViewMatrix);

  fragPos = vec3(model * vec4(vertex_position, 1.0));
  vec3 normal = t_i_model * vertex_normal;
  vec3 tangent = t_i_model * vertex_tangent;
  texCoord = vertex_texCoords;

  TBN = mat3(tangent,normalize(cross(normal,tangent)),normal);
  // Convert position to clip coordinates and pass along
  gl_Position = proj * view * model * vec4(vertex_position,1.0);
}


  