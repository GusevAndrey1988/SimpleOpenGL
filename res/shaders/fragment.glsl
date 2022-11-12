#version 460

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
    fragColor = texture(tex, texCoord);
}