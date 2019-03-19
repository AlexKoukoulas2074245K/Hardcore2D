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
out vec4 frag_color;

vec3 rgb2hsv(vec3 inputRGB)
{
    vec3    outputHSV;
    float  min;
	float  max;
	float  delta;

    min = inputRGB.r < inputRGB.g ? inputRGB.r : inputRGB.g;
    min = min  < inputRGB.b ? min  : inputRGB.b;

    max = inputRGB.r > inputRGB.g ? inputRGB.r : inputRGB.g;
    max = max  > inputRGB.b ? max  : inputRGB.b;

    outputHSV.b = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        outputHSV.g = 0;
        outputHSV.r = 0; // undefined, maybe nan?
        return outputHSV;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        outputHSV.g = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        outputHSV.g = 0.0;
        outputHSV.r = 0.0;                            // its now undefined
        return outputHSV;
    }
    if( inputRGB.r >= max )                           // > is bogus, just keeps compilor happy
        outputHSV.r = ( inputRGB.g - inputRGB.b ) / delta;        // between yellow & magenta
    else
    if( inputRGB.g >= max )
        outputHSV.r = 2.0 + ( inputRGB.b - inputRGB.r ) / delta;  // between cyan & yellow
    else
        outputHSV.r = 4.0 + ( inputRGB.r - inputRGB.g ) / delta;  // between magenta & cyan

    outputHSV.r *= 60.0;                              // degrees

    if( outputHSV.r < 0.0 )
        outputHSV.r += 360.0;

    return outputHSV;
}

vec3 hsv2rgb(vec3 inputHSV)
{
    float      hh, p, q, t, ff;
    int        i;
    vec3         outputRGB;

    if(inputHSV.g <= 0.0) {       // < is bogus, just shuts up warnings
        outputRGB.r = inputHSV.b;
        outputRGB.g = inputHSV.b;
        outputRGB.b = inputHSV.b;
        return outputRGB;
    }
    hh = inputHSV.r;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = int(hh);
    ff = hh - i;
    p = inputHSV.b * (1.0 - inputHSV.g);
    q = inputHSV.b * (1.0 - (inputHSV.g * ff));
    t = inputHSV.b * (1.0 - (inputHSV.g * (1.0 - ff)));

    switch(i) {
    case 0:
        outputRGB.r = inputHSV.b;
        outputRGB.g = t;
        outputRGB.b = p;
        break;
    case 1:
        outputRGB.r = q;
        outputRGB.g = inputHSV.b;
        outputRGB.b = p;
        break;
    case 2:
        outputRGB.r = p;
        outputRGB.g = inputHSV.b;
        outputRGB.b = t;
        break;

    case 3:
        outputRGB.r = p;
        outputRGB.g = q;
        outputRGB.b = inputHSV.b;
        break;
    case 4:
        outputRGB.r = t;
        outputRGB.g = p;
        outputRGB.b = inputHSV.b;
        break;
    case 5:
    default:
        outputRGB.r = inputHSV.b;
        outputRGB.g = p;
        outputRGB.b = q;
        break;
    }
    return outputRGB;     
}

vec4 changeHue(vec4 color)
{
	vec3 hsv_rep = rgb2hsv(color.rgb);	
	hsv_rep.r = 200.0;
	return vec4(hsv2rgb(hsv_rep).rgb, color.a);
}

void main()
{
    float final_uv_x = uv_frag.x;
    if (flip_tex_hor) final_uv_x = 1.00 - final_uv_x;

    float final_uv_y = 1.00 - uv_frag.y;
    if (flip_tex_ver) final_uv_y = 1.00 - final_uv_y;

    frag_color = texture(tex, vec2(final_uv_x, final_uv_y));
	frag_color = changeHue(frag_color);	
}
