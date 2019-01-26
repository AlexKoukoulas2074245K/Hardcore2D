#version 330 core

// has to have same name as vertex shader
in vec2 uv_frag;

// our texture
uniform sampler2D tex;
uniform float swirlRadius;
uniform float swirlAngle;
uniform vec2 swirlCenter;
uniform vec2 swirlDimensions;

// actual output
// gl_FragColor is deprecated
out vec4 frag_color;

vec4 swirlEffect()
{
    vec2 tc = uv_frag * swirlDimensions;
    tc -= swirlCenter;
    float dist = length(tc);
    if (dist < swirlRadius) 
    {
        float percent = (swirlRadius - dist)/swirlRadius;
        float theta = percent * percent * swirlAngle * 8.0;
        float angleSine = sin(theta);
        float angleCosine = cos(theta);
        tc = vec2(dot(tc, vec2(angleCosine, -angleSine)), dot(tc, vec2(angleSine, angleCosine)));
    }

    tc += swirlCenter;
    return vec4(texture(tex, tc/swirlDimensions).rgb, 1.);
}

void main()
{
    frag_color = swirlEffect();
}