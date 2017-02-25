/*
* @Author: krocki
* @Date:   2017-02-23 20:47:29
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-02-24 16:00:15
*/

#include <thread>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#ifdef NANOVG_GLEW
	#include <GL/glew.h>
#endif
#ifdef __APPLE__
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include <nanovg/nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>
#include "demo.h"
#include "perf.h"



#define DEMO_MSAA

void errorcb ( int error, const char *desc ) {
	printf ( "GLFW error %d: %s\n", error, desc );
}

int blowup = 0;
int screenshot = 0;
int premult = 0;
#define N 4
double cpuTime[N + 1];
std::thread threads[N];
double tic[N + 1], toc[N + 1];
PerfGraph fps, cpuGraph[N + 1], gpuGraph;
GLFWwindow *window;
DemoData data;
NVGcontext *vg = NULL;
GPUtimer gpuTimer;

float rand_float ( float mn, float mx ) {
	float r = random() / ( float ) RAND_MAX;
	return mn + ( mx - mn ) * r;
}

double get_time ( void ) {

	struct timeval tv;
	gettimeofday ( &tv, NULL );
	double t;
	t = tv.tv_sec + tv.tv_usec * 1e-6;
	return t;
	
}

static void key ( GLFWwindow *window, int key, int scancode, int action, int mods ) {
	NVG_NOTUSED ( scancode );
	NVG_NOTUSED ( mods );
	
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose ( window, GL_TRUE );
		
	if ( key == GLFW_KEY_SPACE && action == GLFW_PRESS )
		blowup = !blowup;
		
	if ( key == GLFW_KEY_S && action == GLFW_PRESS )
		screenshot = 1;
		
	if ( key == GLFW_KEY_P && action == GLFW_PRESS )
		premult = !premult;
}

int test ( int i ) {

	struct timeval tv;
	
	long wait = 10000;
	
	while ( !glfwWindowShouldClose ( window ) ) {
	
		toc[i + 1] = get_time();
		cpuTime[i + 1] = toc[i + 1] - tic[i + 1];
		tic[i + 1] = get_time();
		// how busy is this thread
		double perc_free = 100.0 * ( ( double ) wait / ( double ) 1e6 ) / cpuTime[i + 1];
		printf ( "%d %f %f\n", i, perc_free, cpuTime[i + 1] );
		updateGraph ( &cpuGraph[i + 1], perc_free );
		
		usleep ( wait );
		
	}
	
	return 0;
	
}


int main() {

	double prevt = 0;
	
	if ( !glfwInit() ) {
		printf ( "Failed to init GLFW." );
		return -1;
	}
	
	initGraph ( &fps, GRAPH_RENDER_FPS, "Frame Time" );
	initGraph ( &cpuGraph[0], GRAPH_RENDER_MS, "GLFW Thread" );
	
	for ( int i = 0; i < N; i++ ) {
	
		char str[32];
		sprintf ( str, "Aux Thread %d Time", i );
		initGraph ( &cpuGraph[i + 1], GRAPH_RENDER_PERCENT, str );
		
	}
	
	initGraph ( &gpuGraph, GRAPH_RENDER_MS, "GPU Time" );
	
	glfwSetErrorCallback ( errorcb );
	#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	#endif
	glfwWindowHint ( GLFW_OPENGL_DEBUG_CONTEXT, 1 );
	
	#ifdef DEMO_MSAA
	glfwWindowHint ( GLFW_SAMPLES, 4 );
	printf ( "MSAA Enabled\n" );
	#endif
	window = glfwCreateWindow ( 400, 600, "NanoVG", NULL, NULL );
	
	for ( int i = 0; i < N; i++ ) {
		tic[i] = get_time();
		threads[i] = std::thread ( test, i );
	}
	
	//	window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
	if ( !window ) {
		glfwTerminate();
		return -1;
	}
	
	glfwSetKeyCallback ( window, key );
	
	glfwMakeContextCurrent ( window );
	#ifdef NANOVG_GLEW
	glewExperimental = GL_TRUE;
	
	if ( glewInit() != GLEW_OK ) {
		printf ( "Could not init glew.\n" );
		return -1;
	}
	
	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();
	#endif
	
	#ifdef DEMO_MSAA
	vg = nvgCreateGL3 ( NVG_STENCIL_STROKES | NVG_DEBUG );
	#else
	vg = nvgCreateGL3 ( NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG );
	#endif
	
	if ( vg == NULL ) {
		printf ( "Could not init nanovg.\n" );
		return -1;
	}
	
	if ( loadDemoData ( vg, &data ) == -1 )
		return -1;
		
	glfwSwapInterval ( 0 );
	
	initGPUTimer ( &gpuTimer );
	
	glfwSetTime ( 0 );
	prevt = glfwGetTime();
	
	while ( !glfwWindowShouldClose ( window ) ) {
	
		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;
		float gpuTimes[3];
		int i, n;
		
		t = glfwGetTime();
		dt = t - prevt;
		prevt = t;
		
		startGPUTimer ( &gpuTimer );
		
		glfwGetCursorPos ( window, &mx, &my );
		glfwGetWindowSize ( window, &winWidth, &winHeight );
		glfwGetFramebufferSize ( window, &fbWidth, &fbHeight );
		// Calculate pixel ration for hi-dpi devices.
		pxRatio = ( float ) fbWidth / ( float ) winWidth;
		
		// Update and render
		glViewport ( 0, 0, fbWidth, fbHeight );
		
		if ( premult )
			glClearColor ( 0, 0, 0, 0 );
		else
			glClearColor ( 0.3f, 0.3f, 0.32f, 1.0f );
			
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		nvgBeginFrame ( vg, winWidth, winHeight, pxRatio );
		
		render_minimum ( vg, mx, my, winWidth, winHeight, t, blowup, &data );
		
		renderGraph ( vg, 5, 5, &fps );
		//this thread
		// renderGraph ( vg, 5 + 200 + 5, 5, 		&cpuGraph[0] );
		
		for ( size_t i = 0; i < N + 1; i++ )
			renderGraph ( vg, 5, 40 * ( i + 1 ) + 5, 	&cpuGraph[i] );
			
		nvgEndFrame ( vg );
		
		// Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
		cpuTime[0] = glfwGetTime() - t;
		
		updateGraph ( &fps, dt );
		
		updateGraph ( &cpuGraph[0], cpuTime[0] );
		
		// We may get multiple results.
		n = stopGPUTimer ( &gpuTimer, gpuTimes, 3 );
		
		for ( i = 0; i < n; i++ )
			updateGraph ( &gpuGraph, gpuTimes[i] );
			
		if ( screenshot ) {
			screenshot = 0;
			saveScreenShot ( fbWidth, fbHeight, premult, "dump.png" );
		}
		
		glfwSwapBuffers ( window );
		glfwPollEvents();
	}
	
	freeDemoData ( vg, &data );
	
	nvgDeleteGL3 ( vg );
	
	printf ( "Average Frame Time: %.2f ms\n", getGraphAverage ( &fps ) * 1000.0f );
	
	for ( int i = 0; i < N; i++ )
		printf ( "          CPU %d Time: %.2f ms\n", i, getGraphAverage ( &cpuGraph[i] ) * 1000.0f );
		
	printf ( "          GPU Time: %.2f ms\n", getGraphAverage ( &gpuGraph ) * 1000.0f );
	
	glfwTerminate();
	
	for ( int i = 0; i < N; i++ )
		threads[i].join();
		
	return 0;
}
