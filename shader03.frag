/************************************************************************/
/*    Graphics 317 coursework exercise 03                               */
/*    Author: Bernhard Kainz                                            */
/*    Nothing to be done in this file for this exercise                 */
/************************************************************************/

#version 150 compatibility

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularExponent;

in fragmentData
{
	vec3 vpos;
	vec3 normal;
	vec4 color;
}frag;

///////////////

void main()
{
	vec4 outccol;
  //simple shading
  outccol = ambientColor +  clamp( diffuseColor * 
    max(dot(frag.normal,normalize(gl_LightSource[0].position.xyz - frag.vpos)), 0.0), 0.0, 1.0 ) ;
	gl_FragColor = outccol;
}
