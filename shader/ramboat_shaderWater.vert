
precision lowp float;

attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

attribute float a_topV;
varying mediump float v_topV;

varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
varying mediump vec2 v_texCoordRefraction;

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
	v_fragmentColor = a_color;
	v_texCoord = a_texCoord;
    v_texCoordRefraction = a_texCoord + vec2(CC_SinTime.x,CC_CosTime.x);
    v_topV = a_topV;
}