#version 330 core

// has to have same name as vertex shader
in vec2 uv_frag;

// our texture
uniform sampler2D tex;
uniform float swirlRadius;
uniform float swirlAngle;
uniform float blurIntensity;
uniform vec2 swirlCenter;
uniform vec2 swirlDimensions;

// actual output
// gl_FragColor is deprecated
out vec4 frag_color;

// Swirl post-processing effect
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

// Box blur post-processing effect
float mapBlurIntensityToPixelContribution(float intensity)
{	
	return 0.111111 + 0.888888 * (1.0 - intensity);
}

vec4 blurEffect(vec4 frag_color)
{
	float hor_offset = 0.0015625;
	float ver_offset = 0.00277777777;
		
	float mainPixelContribution = mapBlurIntensityToPixelContribution(blurIntensity);
	float otherPixelsContribution = (1.0 - mainPixelContribution)/8.0;
	
	vec4 blurredColor = 
		frag_color * mainPixelContribution +
		vec4(texture(tex, vec2(uv_frag.x - hor_offset, uv_frag.y - ver_offset)).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x             , uv_frag.y - ver_offset)).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x + hor_offset, uv_frag.y - ver_offset)).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x - hor_offset, uv_frag.y             )).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x + hor_offset, uv_frag.y             )).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x - hor_offset, uv_frag.y + ver_offset)).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x             , uv_frag.y + ver_offset)).rgb, 1.) * otherPixelsContribution +
		vec4(texture(tex, vec2(uv_frag.x + hor_offset, uv_frag.y + ver_offset)).rgb, 1.) * otherPixelsContribution;
		
	return blurredColor;
	
}

void main()
{
    frag_color = swirlEffect();
	
}