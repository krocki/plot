/*
* @Author: kmrocki
* @Date:   2017-02-22 11:57:33
* @Last Modified by:   kmrocki
* @Last Modified time: 2017-02-22 15:37:47
*/

#include "maths_funcs.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

int width = 640;
int height = 480;
mat4 persp_proj = identity_mat4 ();

void window_size_callback ( GLFWwindow *window, int w, int h ) {
	width = w;
	height = h;
	float near = 0.1f; // clipping plane
	float far = 100.0f; // clipping plane
	float fov = 67.0f;
	float aspect = ( float ) width / ( float ) height; // aspect ratio
	persp_proj = perspective ( fov, aspect, near, far );
}

int main ( void ) {

	if ( !glfwInit () ) {
		fprintf ( stderr, "ERROR: could not start GLFW3\n" );
		return 1;
	}
	
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	
	
	// GLFWmonitor *mon = glfwGetPrimaryMonitor ();
	GLFWwindow *window = glfwCreateWindow ( width, height, "Multiple plots", NULL, NULL );
	
	if ( !window ) {
		fprintf ( stderr, "ERROR: could not open window with GLFW3\n" );
		glfwTerminate();
		return 1;
	}
	
	glfwMakeContextCurrent ( window );
	glfwSetWindowSizeCallback ( window, window_size_callback );
	
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	
	if ( GLEW_OK != err ) {
		/* Problem: glewInit failed, something is seriously wrong. */
		printf ( "Problem: glewInit failed, something is seriously wrong.\n" );
		return false;
	}
	
	
	// get version info
	const GLubyte *renderer = glGetString ( GL_RENDERER ); // get renderer string
	const GLubyte *version = glGetString ( GL_VERSION ); // version as a string
	
	printf ( "Renderer: %s\n", renderer );
	printf ( "OpenGL version supported %s\n", version );
	
	glEnable ( GL_DEPTH_TEST ); // enable depth-testing
	glDepthFunc ( GL_LESS ); // depth-testing interprets a smaller value as "closer"
	glClearColor ( 0.5f, 0.5f, 0.5f, 1.0f );
	//glEnable(GL_CULL_FACE);
	glCullFace ( GL_BACK );
	glFrontFace ( GL_CCW );
	
	while ( !glfwWindowShouldClose ( window ) ) {
	
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glfwSwapBuffers ( window );
		glfwPollEvents ();
		
		if ( GLFW_PRESS == glfwGetKey ( window, GLFW_KEY_ESCAPE ) )
			glfwSetWindowShouldClose ( window, 1 );
			
			
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
	
}
