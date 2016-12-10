#version 150

in vec2 textCoord;

out vec4 FragmentColor;

uniform sampler2D sampler;

void main() {
  // Lookup the color in Texture on coordinates given by fragTexCoord
//  FragmentColor = texture(Texture, FragTexCoord);
   FragmentColor = texture(sampler,textCoord);
 }
