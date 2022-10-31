#version 330 core

uniform sampler2D tex;

layout(location = 0) out vec4 color;

in vec2 TexCoord;

void main() {
    vec4 in_color = texture(tex, TexCoord);
    color = in_color;
}
