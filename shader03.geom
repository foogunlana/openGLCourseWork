/************************************************************************/
/*    Graphics 317 coursework exercise 03                               */
/*    Author: Bernhard Kainz                                            */
/*    This file has to be altered for this exercise                     */
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


///////////////////////////////////////////////////////
//pseudo random helper function
///////////////////////////////////////////////////////
float rnd(vec2 x)
{
	int n = int(x.x * 40.0 + x.y * 6400.0);
	n = (n << 13) ^ n;
	return 1.0 - float( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}


///////////////////////////////////////////////////////
//TODO add code for exercise 3 Geometry generation here
///////////////////////////////////////////////////////
void produceVertex(float s, float t, vec4 v0, vec4 v01, vec4 v02, vec3 n0, vec3 n01, vec3 n02, int i)
{
  ///////////////////////////////////////////////////////
  //TODO implement for subdivision
  ///////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////
}
///////////////////////////////
void main()
{
	///////////////////////////////////////////////////////
	//TODO replace pass through shader with solution
	///////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////



  ///////////////////////////////////////////////////////
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
