#ifdef GL_ES
varying mediump vec2 v_texCoord;
varying mediump vec4 v_fragmentColor;
#else
varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
#endif

void main()
{
	vec4 total = vec4(0,0,0,1);
	vec4 color = texture2D(CC_Texture0, v_texCoord);  	
	total += texture2D(CC_Texture0, v_texCoord + vec2(-4 * 0.0010416, -4 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(-3 * 0.0010416, -3 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(-2 * 0.0010416, -2 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(-1 * 0.0010416, -1 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(-0 * 0.0010416, -0 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(1 * 0.0010416, 1 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(2 * 0.0010416, 2 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(3 * 0.0010416, 3 * 0.0015625)); 
	total += texture2D(CC_Texture0, v_texCoord + vec2(4 * 0.0010416, 4 * 0.0015625)); 	
	total *= 0.1111111;
	gl_FragColor = vec4(total.r, total.g, total.b, color.a) * v_fragmentColor;
}