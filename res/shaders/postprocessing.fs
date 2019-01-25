#version 330 core

// has to have same name as vertex shader
in vec2 uv_frag;

// our texture
uniform sampler2D tex;

// actual output
// gl_FragColor is deprecated
out vec3 frag_color;

void main()
{
    frag_color = texture(tex, vec2(uv_frag.x, 1.0f - uv_frag.y)).xyz;
}