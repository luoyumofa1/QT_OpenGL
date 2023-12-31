#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
out vec3 Color;
uniform mat4 matrix;
void main(){
    gl_Position=matrix*vec4(aPos.x,aPos.y,aPos.z,1.0f);
    Color=aColor;
}
