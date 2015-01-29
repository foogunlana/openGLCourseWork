/************************************************************************/
/*    Graphics 317 coursework exercise 03                               */
/*    Author: Bernhard Kainz                                            */
/*    This file has to be altered for this exercise                     */
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
	//Exercise 4:
	vec4 texCoords;
}vertex;

/////////////

void main()
{
	vertex.pos = vec3(gl_ModelViewMatrix * gl_Vertex);
	vertex.normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
	vertex.color = vec4(1.0,0.0,0.0,1.0);

  //////////////////////////////////////////////////////////
  // TODO Exercise 04a: add correct coordinate value to 
	// vertex.texCoords = ...
  // and follow the varibale through the pipeline 
  // down to the fragment shader
  //////////////////////////////////////////////////////////


}