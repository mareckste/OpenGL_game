#version 150
// Get vertex position from the vertex buffer
in vec2 Position;
in vec3 Color;

vec3 Color2;

// Matrices as program attributes
uniform mat3 Transform;

// This will be passed on to the fragment shader
out vec3 VertexColor;

void main() {
Color2.x = 0.0;
Color2.y = 0.0;
Color2.z = 1.0;

  VertexColor = Color2;

  // Apply transformation matrix using homogeneous coordinates
    vec3 transformedPosition = Transform * vec3(Position, 1.0);

    // Calculate the final position on screen, this needs to be a 3D homogeneous vector
    //gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0.0, 1.0);

  // The vertex shader needs to output a vec4 gl_Position so we fill in z and w to comply
  gl_Position = vec4(transformedPosition.x, transformedPosition.y,0.0, 1.0);
}