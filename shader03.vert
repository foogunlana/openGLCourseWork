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

out vertexData
{
	vec3 pos;
	vec3 normal;
	vec4 color;
}vertex;

void main()
{
	vertex.pos = vec3(gl_ModelViewMatrix * gl_Vertex);
	vertex.normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
	vertex.color = vec4(1.0,0.0,0.0,1.0);
}