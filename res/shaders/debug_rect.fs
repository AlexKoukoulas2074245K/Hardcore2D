#version 330 core

// has to have same name as vertex shader
in vec2 uv_frag;

// our texture
uniform sampler2D tex;

// Texture flipping booleans
uniform bool flip_tex_hor;
uniform bool flip_tex_ver;

// actual output
// gl_FragColor is deprecated
layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = texture(tex, vec2(uv_frag.x, 1.0f - uv_frag.y));
	frag_color.w = 0.5f;
}