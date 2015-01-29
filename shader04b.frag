/************************************************************************/
/*    Graphics 317 coursework exercise 04b                               */
/*    Author: Bernhard Kainz                                            */
/*    This file has to be altered for this exercise                     */
/************************************************************************/

#version 150 core

//simplified IO structures, no geometry shader any more
in vec2 textureUV;
out vec3 color;

//this is the texture of the framebuffer object
uniform sampler2D renderedTexture;

void main(){

	//this is one fragment of the framebuffer texture, which the main programme produces 
	color = texture( renderedTexture, textureUV).xyz;

  ////////////////////////////////////////////////////////////////////
  // TODO Exercise 04b: implement a simple image based blur in following
  ////////////////////////////////////////////////////////////////////



  ////////////////////////////////////////////////////////////////////

}
