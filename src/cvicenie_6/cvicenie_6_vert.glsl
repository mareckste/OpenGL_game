#version 150
// The inputs will be fed by the vertex buffer objects
in vec3 Position;
in vec2 TexCoord;

// This will be passed to the fragment shader
out vec2 FragTexCoord;

// Matrices as program attributes
uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;
uniform mat3 Transform;


void main() {
  // Apply transformation matrix using homogeneous coordinates
  vec4 transformedPosition = Projection * View * Model * vec4(Position, 1.0f);

  // Copy the texture coordinates to the fragment shader
  FragTexCoord = vec2(TexCoord.x, TexCoord.y);

  // Calculate the final position on screen, this needs to be a 3D homogeneous vector
  gl_Position = transformedPosition;
}
