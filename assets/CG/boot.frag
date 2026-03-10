uniform sampler2D texture;
uniform float u_time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 base = texture2D(texture, uv);

    // 中心距离
    float dist = distance(uv, vec2(0.5));

    // 强烈发光
    float glow = 0.3 / (dist * 5.0 + 0.2);

    // 呼吸亮度
    float pulse = 1.0 + 0.2 * sin(u_time * 4.0);

    vec3 color = base.rgb * pulse + glow;

    gl_FragColor = vec4(color, base.a);
}