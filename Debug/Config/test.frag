uniform sampler2D tex;

void main()
{
	float mipmaplevel = 0.0;
	vec4 color = textureLod(tex, gl_TexCoord[0].st, mipmaplevel);
	gl_FragColor = color;
}