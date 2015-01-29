/************************************************************************/
/*    Graphics 317 coursework exercise 02                               */
/*    Author: Bernhard Kainz                                            */
/*    Nothing to be done in this file for this exercise                 */
/************************************************************************/

#version 150 compatibility
#extension GL_ARB_geometry_shader4 : enable

layout (max_vertices = 72) out;

const float pi = 3.14159265359;

////////////////
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularExponent;

uniform int level;
uniform float time;

in vertexData
{
	vec3 pos;
	vec3 normal;
	vec4 color;
}vertices[];

out fragmentData
{
	vec3 vpos;
	vec3 normal;
	vec4 color;
}frag;   


///////////////////////////////
void main()
{
  // This is a pass-through shader copying the vertices
  // as they come in
	int i;
	for(i = 0; i < gl_in.length(); i++)
	{
	frag.vpos = vertices[i].pos;
	frag.normal = vertices[i].normal;
	frag.color = vertices[i].color;
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();
	}
	EndPrimitive();
	
}
