/************************************************************************/
/*    Graphics 317 coursework exercise 05                               */
/*    Author: Bernhard Kainz                                            */
/*    This file has to be altered for this exercise                     */
/************************************************************************/


#version 150 compatibility

in vec3 origin, dir, point; 
out vec4 outcolour;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

const int raytraceDepth = 42;
const int numSpheres = 6;

struct Ray
{
  vec3 origin;
  vec3 dir;
};
struct Sphere
{
  vec3 centre;
  float radius;
  vec3 colour;
};
struct Plane
{
  vec3 point;
  vec3 normal;
  vec3 colour;
};

struct Intersection
{
  float t;
  vec3 point;     // hit point
  vec3 normal;     // normal
  int hit;
  vec3 colour;
};

////////////////////////////////////////////////////////////////////
// TODO Exercise 5: implement a simple geometry based ray tracing
// implement the functions in the follwing. 
// In order to reach all points you need to implement at least one
// feature more than shown in the coursework description
// effects like refraction, scattering, caustics, soft hadows, etc. 
// are possible. 
////////////////////////////////////////////////////////////////////

void shpere_intersect(Sphere sph, Ray ray, inout Intersection intersect)
{
  ////////////////////////////////////////////////////////////////////
  // TODO 
  ////////////////////////////////////////////////////////////////////
}

void plane_intersect(Plane pl, Ray ray, inout Intersection intersect)
{
  ////////////////////////////////////////////////////////////////////
  // TODO 
  ////////////////////////////////////////////////////////////////////
}

Sphere sphere[numSpheres];
Plane plane;
void Intersect(Ray r, inout Intersection i)
{
  ////////////////////////////////////////////////////////////////////
  // TODO 
  ////////////////////////////////////////////////////////////////////
}

int seed = 0;
float rnd()
{
  seed = int(mod(float(seed)*1364.0+626.0, 509.0));
  return float(seed)/509.0;
}

vec3 computeShadow(in Intersection intersect)
{
  ////////////////////////////////////////////////////////////////////
  // TODO 
  ////////////////////////////////////////////////////////////////////

  return vec3(0,0,0);
}

void main()
{
  //initial scene definition
  sphere[0].centre   = vec3(-2.0, 1.5, -3.5);
  sphere[0].radius   = 1.5;
  sphere[0].colour = vec3(0.8,0.8,0.8);
  sphere[1].centre   = vec3(-0.5, 0.0, -2.0);
  sphere[1].radius   = 0.6;
  sphere[1].colour = vec3(0.3,0.8,0.3);
  sphere[2].centre   = vec3(1.0, 0.7, -2.2);
  sphere[2].radius   = 0.8;
  sphere[2].colour = vec3(0.3,0.8,0.8);
  sphere[3].centre   = vec3(0.7, -0.3, -1.2);
  sphere[3].radius   = 0.2;
  sphere[3].colour = vec3(0.8,0.8,0.3);
  sphere[4].centre   = vec3(-0.7, -0.3, -1.2);
  sphere[4].radius   = 0.2;
  sphere[4].colour = vec3(0.8,0.3,0.3);
  sphere[5].centre   = vec3(0.2, -0.2, -1.2);
  sphere[5].radius   = 0.3;
  sphere[5].colour = vec3(0.8,0.3,0.8);
  plane.point = vec3(0,-0.5, 0);
  plane.normal = vec3(0, 1.0, 0);
  plane.colour = vec3(1, 1, 1);
  seed = int(mod(dir.x * dir.y * 39786038.0, 65536.0));
  //scene definition end

  //make empty for exercise 
  outcolour = vec4(1,1,1,1);

  ////////////////////////////////////////////////////////////////////
  // TODO 
  ////////////////////////////////////////////////////////////////////
}