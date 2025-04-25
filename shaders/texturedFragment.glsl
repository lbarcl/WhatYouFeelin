#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main(){
    vec2 flipedTexCoord=vec2(TexCoord.x,1.-TexCoord.y);// Flip the texture coordinate vertically
    FragColor=texture(uTexture,flipedTexCoord);
}