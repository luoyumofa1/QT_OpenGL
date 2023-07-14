#version 330 core
out vec4 FragColor;
in vec3 Color;
uniform vec4 ourColor;
void main(){
    gl_FragColor=vec4(Color,1.0f);
}
