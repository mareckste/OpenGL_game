#version 150
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 FragTexCoord;

// The final color
out vec4 FragmentColor;

void main() {
  // Lookup the color in Texture on coordinates given by fragTexCoord
       vec4 color = vec4(0,0,0,0);
       float bias = 0;
       float factor = 1.0;
       float matrix[25] =  float[](0,  0, -1,  0,  0,
                                   0,  0, -1,  0,  0,
                                   0,  0,  4,  0,  0,
                                   0,  0, -1,  0,  0,
                                   0,  0, -1,  0,  0);

       for (int x = 0; x < 5; x++) {
          for (int y = 0; y < 5; y++) {
              color += matrix[x*5+y] * texture(Texture, FragTexCoord + vec2(x*(1/512.0),y*(1/512.0)));

          }
       }
        FragmentColor = color/factor + vec4(bias, bias, bias, 1);

}
