#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_texCoordRefraction;
//uniform sampler2D CC_Texture0;
uniform sampler2D u_refraction;

varying float v_topV;

void main()
{
    // Refracción
#ifdef GL_ES
    mediump vec4 refractionColor = texture2D(u_refraction,v_texCoordRefraction);
    mediump vec2 dispTexOffset = vec2((refractionColor.r-0.49)*0.35,refractionColor.g*0.01);
    mediump vec2 dispTexCoord = v_texCoord + dispTexOffset;
    mediump vec4 texColor = texture2D(CC_Texture0, dispTexCoord);
    mediump float lum = (0.299 * texColor.r) + (0.587 * texColor.g) + (0.114 * texColor.b);
#else
    vec4 refractionColor = texture2D(u_refraction,v_texCoordRefraction);
    vec2 dispTexOffset = vec2((refractionColor.r-0.49)*0.35,refractionColor.g*0.01);
    vec2 dispTexCoord = v_texCoord + dispTexOffset;
    vec4 texColor = texture2D(CC_Texture0, dispTexCoord);
    float lum = (0.299 * texColor.r) + (0.587 * texColor.g) + (0.114 * texColor.b);
#endif
    gl_FragColor = vec4 (v_fragmentColor.rgb*lum + texColor.rgb*(1.0-v_fragmentColor.a),1.0);
    
    // Reflexión
    // dispTexCoord siempre debería ser menor que v_topV, factor será menor cuanto más lejos
    // esté el reflejo del agua
#ifdef GL_ES
    mediump float factor = dispTexCoord.y/v_topV;
    // Nota: v_topV + (v_topV - dispTexCoord.y) = 2.0*v_topV-dispTexCoord.y
    mediump vec2 reflTexCoord = vec2(dispTexCoord.x,2.0*v_topV-dispTexCoord.y);
#else
    float factor = dispTexCoord.y/v_topV;
    // Nota: v_topV + (v_topV - dispTexCoord.y) = 2.0*v_topV-dispTexCoord.y
    vec2 reflTexCoord = vec2(dispTexCoord.x,2.0*v_topV-dispTexCoord.y);
#endif
    gl_FragColor += texture2D(CC_Texture0,reflTexCoord) * (factor * factor * 0.5);
}
