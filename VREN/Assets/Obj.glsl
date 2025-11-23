// VERTEX
#version 400 
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUVs;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uTransform;

out vec2 vUVs;

void main() {
    vUVs = aUVs;
    gl_Position = uProj * uView * uTransform * vec4(aPosition, 1.0);
}

// FRAGMENT
#version 400

in vec2 vUVs;

uniform vec4 uColor;
uniform sampler2D uColorTexture;

out vec4 oColor;

void main() {
    oColor = uColor * texture2D(uColorTexture, vUVs);
}
