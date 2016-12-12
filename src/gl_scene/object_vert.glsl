/*#version 150
// The inputs will be fed by the vertex buffer objects
in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

// This will be passed to the fragment shader
out vec2 FragTexCoord;

// Normal to pass to the fragment shader
out vec4 normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main() {
  // Copy the input to the fragment shader
  FragTexCoord = TexCoord;

  // Normal in world coordinates
  normal = normalize(ModelMatrix * vec4(Normal, 0.0f));

  // Calculate the final position on screen
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
*/
#version 150

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec2 FragTexCoord;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main() {

  FragTexCoord = TexCoord;
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);

  fragPosition = vec3(ModelMatrix * vec4(Position, 1.0f));
  normal = mat3(transpose(inverse(ModelMatrix))) * Normal;

}