/************************************************************************/
/*    Graphics 317 coursework exercise 02                               */
/*    Author: Bernhard Kainz                                            */
/*    This file has to be altered for this exercise                     */
/************************************************************************/

#version 150 compatibility

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularExponent;
uniform int shader;

in fragmentData
{
  vec3 pos;
  vec3 normal;
  vec4 color;
}frag;

///////////////

void main()
{
  vec4 outcol = frag.color;

  if(shader == 2)
  {
    ///////////////////////////////////////////////////
    //TODO add code for exercise 2.2 Phong shading here
    ///////////////////////////////////////////////////


    ///////////////////////////////////////////////////
  }

  if(shader == 3)
  {
    ///////////////////////////////////////////////////
    //TODO add code for exercise 2.3 toon shading here
    ///////////////////////////////////////////////////


    ///////////////////////////////////////////////////
  }

  gl_FragColor = outcol;
}
