/************************************************************************/
/*    Graphics 317 coursework exercise 05                              */
/*    Author: Bernhard Kainz                                            */
/*    Nothing to be done in this file for this exercise                 */
/************************************************************************/


#version 150 compatibility
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosInmodelspace;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

out vec3 origin, dir, point; 

void  main () 
{ 
	//nothing to be done here
    gl_Position  = vec4(vertexPosInmodelspace,1) ; 
    origin =  vec3 ( 0, 0, 0 ); 
    dir = normalize ( vec3 ( vertexPosInmodelspace.x * 1.6,  vertexPosInmodelspace.y , -1.0 )); 
	point = vertexPosInmodelspace;
}