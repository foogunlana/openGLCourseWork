/************************************************************************/
/*    Graphics 317 coursework exercise 05                                */
/*    Author: Bernhard Kainz                                            */
/*    Nothing to be done in this file for this exercise                 */
/************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "GL/glew.h"

#ifdef __linux__
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "inc/lodepng.h"

///////////////////////////////////////////////////////////////////////
//Error checking functions
void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
		abort();
	}
}

#define _DEBUG 1
#if _DEBUG
#define GL_CHECK(stmt) do { \
	stmt; \
	CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

#define GET_GLERROR(ret)                                          \
{                                                                 \
	GLenum err = glGetError();                                    \
	if (err != GL_NO_ERROR) {                                     \
	fprintf(stderr, "[%s line %d] GL Error: %s\n",                \
	__FILE__, __LINE__, gluErrorString(err));                     \
	fflush(stderr);                                               \
	}                                                             \
}

void CheckFramebufferStatus()
{
	GLenum status;
	status = (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
	switch(status) {
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("Unsupported framebuffer format\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("Framebuffer incomplete, missing attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		printf("Framebuffer incomplete, attached images must have same dimensions\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		printf("Framebuffer incomplete, attached images must have same format\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("Framebuffer incomplete, missing draw buffer\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("Framebuffer incomplete, missing read buffer\n");
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////
//Text file IO for shader files

char *textFileRead(char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}

	if (content == NULL)
	{
		fprintf(stderr, "ERROR: could not load in file %s\n", fn);
		exit(1);
	}
	return content;
}

//////////////////////////////////////////////////////////////////////
//shaders and light pos
GLuint v,f,p,rf,pr;
float lpos[4] = {5.0, 0.5, 5.0, 0.0};

GLuint fb;  // color render target
GLuint depth_rb; // depth render target
GLuint stencil_rb; // depth render target
GLuint tex; // texture

int texWidth  = 256;
int texHeight = 256;

// mouse controls
int mouse_old_x, mouse_old_y;
int mouse_buttons = 0;
float rotate_x = 0.0, rotate_y = 0.0;
float move_x = 0.0, move_y = 0.0;
float translate_z = -1.0;
float win_width = 128.0, win_height = 128.0;
unsigned int frameCaptured = 0;
/////////////////////////////////////////////////

void captureFrame()
{
  int dim = 3 * win_width * win_height;
  unsigned char* pixels = new unsigned char[dim];
  glReadPixels(0, 0, win_width, win_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  encodeOneStep(pixels, win_width, win_height, std::string("05"));
  delete[] pixels;
}

void intiFB(int w, int h)
{
	////////framebuffer init
	if(glIsTexture(tex))
	{
		glDeleteRenderbuffers(1, &depth_rb);
		glDeleteTextures(1,&tex);
		glDeleteFramebuffers(1, &fb);
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenFramebuffers(1, &fb);
	glGenTextures(1, &tex);
	glGenRenderbuffers(1, &depth_rb);

	glBindFramebuffer(GL_FRAMEBUFFER, fb);    

	// init texture
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, 
		GL_RGBA, GL_FLOAT, NULL);
	GET_GLERROR(NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, tex, 0);

	GET_GLERROR(0);

	// initialize depth renderbuffer
	glBindRenderbufferEXT(GL_RENDERBUFFER, depth_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, texWidth, texHeight);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
		GL_RENDERBUFFER, depth_rb);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);

	GET_GLERROR(0);

	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

///////////////////////////////////////////////////////////////////////
//adapt viewport when window size changes
void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);

	texWidth = w;
	texHeight = h;
	intiFB(texWidth, texHeight);
	win_width = w;
	win_height = h;
}

///////////////////////////////////////////////////////////////////////
//the actual render function, which is called for each frame
void renderScene(void)
{
  GLfloat modelViewMatrix[16]; 
  GLfloat projectionMatrix[16]; 

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	{
	//render to texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,0.0,1.0,0.0,0.0,-1.0,0.0f,1.0f,0.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glPushMatrix();
	/////////////////////////////////////////////////
	//TODO add scene interaction code here
	glTranslatef(move_x, move_y, 0.0);
	glTranslatef(0.0, 0.0, translate_z);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix); 
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix); 
	/////////////////////////////////////////////////

	glPopMatrix();

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// now render to the screen using the texture...
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	 
	// draw textured quad
	glUseProgram(p);

	GLint loc = glGetUniformLocation(p, "modelViewMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv( loc, 1, GL_FALSE, modelViewMatrix); 
	}
	loc = glGetUniformLocation(p, "projectionMatrix");
	if (loc != -1)
	{
		glUniformMatrix4fv( loc, 1, GL_FALSE, projectionMatrix); 
	}
	    
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_TEXTURE_2D);  

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0,         0);        glVertex2f(-1, -1);
		glTexCoord2f(texWidth, 0);         glVertex2f( 1, -1);
		glTexCoord2f(texWidth, texHeight); glVertex2f( 1,  1);
		glTexCoord2f(0,        texHeight); glVertex2f(-1,  1);
	}
	glEnd();

	glPopMatrix();
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glUseProgram(0);

  //helper function for submission. Will capture 2nd frame of task
  frameCaptured++;
  if (frameCaptured == 2)
  {
    renderID(win_width, win_height);
    captureFrame();
  }

	GL_CHECK(glutSwapBuffers());
}

///////////////////////////////////////////////////////////////////////
//keyboard functions
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

///////////////////////////////////////////////////////////////////////
// mouse interaction functions
void mouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		mouse_buttons |= 1<<button;
	}
	else if (state == GLUT_UP)
	{
		mouse_buttons = 0;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

void mouseMotion(int x, int y)
{
	float dx, dy;
	dx = (float)(x - mouse_old_x);
	dy = (float)(y - mouse_old_y);

	if (mouse_buttons & 1)
	{
		rotate_x += dy * 0.2f;
		rotate_y += dx * 0.2f;
	}
	else if (mouse_buttons & 2)
	{
		move_x += dx * 1.0f/win_width;
		move_y -= dy * 1.0f/win_height;
	}
	else if (mouse_buttons & 4)
	{
		translate_z += dy * 0.01f;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

///////////////////////////////////////////////////////////////////////
//load, compile and set the shaders
void setShaders()
{
	char *vs,*fs,*rfs;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	rf = glCreateShader(GL_FRAGMENT_SHADER);

  vs = textFileRead(const_cast<char *>("./shader05.vert"));
  fs = textFileRead(const_cast<char *>("./shader05.frag"));
	
	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	GLint blen = 0; 
	GLsizei slen = 0;

	glGetShaderiv(v, GL_INFO_LOG_LENGTH , &blen);       
	if (blen > 1)
	{
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetShaderInfoLog(v, blen, &slen, compiler_log);
		std::cout << "compiler_log vertex shader:\n" << compiler_log << std::endl;
		free (compiler_log);
	}
	blen = 0; 
	slen = 0;
	glGetShaderiv(f, GL_INFO_LOG_LENGTH , &blen);       
	if (blen > 1)
	{
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetShaderInfoLog(f, blen, &slen, compiler_log);
		std::cout << "compiler_log fragment shader:\n" << compiler_log << std::endl;
		free (compiler_log);
	}

	p = glCreateProgram();

	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
	//set uniforms

	glUseProgram(0);
}



void initialize () 
{	
	GLfloat aspect = (GLfloat) 568 / 320;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, aspect, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel( GL_SMOOTH );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	GLfloat amb_light[] = { 0.6, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
	glShadeModel( GL_SMOOTH );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 

	intiFB(texWidth, texHeight);

}

///////////////////////////////////////////////////////////////////////
//main, setup and execution of environment
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(512,320);
	glutCreateWindow("Computer Graphics");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);

	glewInit();
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL\n");
	else {
		printf("No GLSL support\n");
		exit(1);
	}

	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 not supported\n");
		exit(1);
	}
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");
	else {
		printf("Not totally ready :( \n");
		exit(1);
	}

	if (GL_ARB_fragment_program && GL_ARB_vertex_program && GL_EXT_framebuffer_object)
		printf("Ready for FrameBuffer\n");
	else {
		printf("Not totally ready :( \n");
		exit(1);
	}


	initialize();
#ifdef __linux__
  int i=pthread_getconcurrency();
#endif
	setShaders();

	glutMainLoop();
	return EXIT_SUCCESS;
}
