#version 150
// The inputs will be fed by the vertex buffer objects
in vec3 Position;
in vec2 textC;
// This will be passed to the fragment shader

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out vec2 textCoord;

void main()
{
  // Apply transformation matrix using homogeneous coordinates
  vec4 transformedPosition = projection * view * model * vec4(Position, 1.0f);
  textCoord = vec2(textC.x,1.0 - textC.y);
  // Calculate the final position on screen, this needs to be a 3D homogeneous vector
  gl_Position = transformedPosition;

}

