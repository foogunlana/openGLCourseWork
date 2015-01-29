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

uniform sampler2D textureImage;

in fragmentData
{
  vec3 vpos;
  vec3 normal;
  vec4 color;
  //Exercise 4:
  vec4 texCoords;
}frag;

///////////////

void main()
{
  //texture information
  vec4 outcol = texture2D(textureImage, frag.texCoords.st);

  //////////////////////////////////////////////////////////
  //TODO Exercise 04a: integrate the texture information 
  // into a Phong shader (e.g. into the one from Exercise 2)
  //////////////////////////////////////////////////////////


    gl_FragColor = vec4(1,0,0,0);
  //////////////////////////////////////////////////////////


}
