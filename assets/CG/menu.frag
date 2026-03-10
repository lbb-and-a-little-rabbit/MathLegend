uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;

#define PI 3.1415926

// ------------------------------------------------
// hash
// ------------------------------------------------

float hash(float n)
{
    return fract(sin(n)*43758.5453123);
}

float hash(vec2 p)
{
    return fract(43758.5453*sin(dot(p,vec2(23.43,91.17))));
}

// ------------------------------------------------
// noise
// ------------------------------------------------

float noise(vec2 x)
{
    vec2 i = floor(x);
    vec2 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float a = hash(i);
    float b = hash(i+vec2(1,0));
    float c = hash(i+vec2(0,1));
    float d = hash(i+vec2(1,1));

    return mix(mix(a,b,f.x),mix(c,d,f.x),f.y);
}

// ------------------------------------------------
// fbm
// ------------------------------------------------

float fbm(vec2 p)
{
    float v = 0.0;
    float a = .5;

    for(int i=0;i<6;i++)
    {
        v += a*noise(p);
        p *= 2.0;
        a *= .5;
    }

    return v;
}

// ------------------------------------------------
// cloud
// ------------------------------------------------

float cloud(vec2 uv)
{
    vec2 p = uv*1.5;

    float c = fbm(p + time*0.02);
    c += 0.5*fbm(p*2.0);

    return smoothstep(.4,.8,c);
}

// ------------------------------------------------
// wind field
// ------------------------------------------------

vec2 wind(vec2 p)
{
    float a = noise(p*0.5+time*0.1)*PI*2.0;
    return vec2(cos(a),sin(a))*0.1;
}

// ------------------------------------------------
// snow particle
// ------------------------------------------------

float snow(vec2 uv, float scale)
{
    uv *= scale;

    vec2 id = floor(uv);
    vec2 gv = fract(uv)-0.5;

    float n = hash(id);

    float t = time*(0.2+n*0.8);

    vec2 p = vec2(n-0.5, fract(n*7.0-t)-0.5);

    p += wind(id);

    vec2 mouseUV = mouse/resolution;

    p += (p-mouseUV)*0.3/(distance(p,mouseUV)+0.2);

    float d = length(gv-p);

    float m = smoothstep(0.04,0.0,d);

    float glow = 0.002/(d*d+0.0005);

    return m+glow;
}

// ------------------------------------------------
// volumetric snow
// ------------------------------------------------

float volumetricSnow(vec2 uv)
{
    float s = 0.0;

    s += snow(uv,12.0);
    s += snow(uv,20.0);
    s += snow(uv,35.0);

    return s;
}

// ------------------------------------------------
// light scattering
// ------------------------------------------------

vec3 scattering(vec2 uv)
{
    float c = cloud(uv);

    vec3 sky = vec3(.02,.03,.05);

    vec3 light = vec3(1.0,.82,.35);

    return mix(sky,light,c*0.6);
}

// ------------------------------------------------
// DOF
// ------------------------------------------------

float depth(vec2 uv)
{
    return smoothstep(.0,1.0,uv.y);
}

// ------------------------------------------------
// bloom helper
// ------------------------------------------------

vec3 bloom(vec3 col)
{
    float l = max(max(col.r,col.g),col.b);

    if(l>0.8)
        col += col*0.8;

    return col;
}

// ------------------------------------------------
// main
// ------------------------------------------------

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution;

    uv.x *= resolution.x/resolution.y;

    vec3 col = scattering(uv);

    float snow = volumetricSnow(uv);

    vec3 gold = vec3(1.0,0.82,0.35);

    col += gold*snow;

    float d = depth(uv);

    col *= mix(.6,1.2,d);

    col = bloom(col);

    float alpha = clamp(snow*0.5,0.0,0.7);

    gl_FragColor = vec4(col,alpha);
}