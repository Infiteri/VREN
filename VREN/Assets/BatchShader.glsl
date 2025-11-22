// VERTEX
#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in mat4 aModel;
layout(location = 5) in vec4 aColor;

uniform mat4 uView;
uniform mat4 uProj;

out vec4 vColor;

void main()
{
    vColor = aColor;
    gl_Position = uProj * uView * aModel * vec4(aPos, 1.0);
}

// FRAGMENT
#version 430 core

in vec4 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vColor;
}
