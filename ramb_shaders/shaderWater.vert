attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

attribute float a_topV;
#ifdef GL_ES
varying mediump float v_topV;
#else
varying float v_topV;
#endif

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
varying mediump vec2 v_texCoordRefraction;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_texCoordRefraction;
#endif

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
	v_fragmentColor = a_color;
	v_texCoord = a_texCoord;
    v_texCoordRefraction = a_texCoord + vec2(CC_SinTime.x,CC_CosTime.x);
    v_topV = a_topV;
}