/*
* @Author: kmrocki
* @Date:   2017-02-22 11:57:33
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-02-23 11:24:24
*/

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <cmath>
#include <string>
#include <functional>
#include <shaders.h>

int width;
int height;
GLFWwindow *window;
bool fullscreen = false;

#define N 4

//init gl vars: buffers
GLuint vao;

GLuint buffer_3d_1[N];
GLuint buffer_3d_2[N];
GLuint buffer_2d[N];

GLuint buffer_axis;
GLuint buffer_axis_marks;
//shader program
GLuint program;
//uniform locations
GLuint uniform_offset;
GLuint uniform_angle;
GLuint uniform_color;
GLuint uniform_rotation;
GLuint uniform_perspective;

//number of graph samples to make
int graph_samples = 35;
//size of axis & marks
int graph_size = 10;
//scale y axis (because otherwise its between 0-1 while x is -30 to +30)
int graph_y_scale = 25;

bool auto_rotate = false;

//initial position + angle variables for shader
float xpos = 0;
float ypos = 0;
float zpos = -40;
float xang = 0;
float yang = 1.05;

//do we draw 2d, 3d?
int draw_2d = 1;
int draw_3d = 1;

//a point in 3d space (used on 2d & 3d graphs)
struct position {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//axis
struct position axis[6];
struct position axis_marks[600];

void window_size_callback ( GLFWwindow *window, int w, int h ) {
	width = w;
	height = h;
	glViewport ( 0, 0, width, height );
	
}

struct position f01 ( float i, float j, float scale ) {

	struct position out;
	
	out.x = i;
	out.z = j;
	
	float d = sqrt ( i * i + j * j );
	
	if ( d == 0.0 )
		d = 1;
		
	out.y = ( sin ( d ) / d ) * scale;
	
	return out;
}

struct position f02 ( float i, float j, float scale ) {

	struct position out;
	
	out.x = i;
	out.z = j;
	
	float x = out.x;
	float z = out.z;
	
	out.y = exp ( sin ( x * 0.5 ) * sin ( z * 0.1 ) ) * 0.9 * exp ( sin ( z * 0.5 ) * sin ( x * 0.1 ) ) * 0.9;
	
	return out;
}

struct position f03 ( float i, float j, float scale ) {

	struct position out;
	
	out.x = i;
	out.z = j;
	
	float s = 0.3;
	out.y = cos ( fabs ( out.x ) * s + fabs ( out.z ) * s ) * ( fabs ( out.x ) * s + fabs ( out.z ) * s );
	
	return out;
}

struct position f04 ( float i, float j, float scale ) {

	struct position out;
	
	
	out.x = i;
	out.z = j;
	
	float x = out.x;
	float z = out.z;
	float r = sqrtf ( 0.02 * x * x + 0.02 * z * z );
	
	out.y = sin ( x * x + 0.1 * z * z ) / ( 0.1 + r * r ) + ( x * x + 1.9 * z * z ) * exp ( 1 - r * r ) / 4.0;
	
	return out;
}

std::function<struct position ( float, float, float )> funcs[] = {f01, f02, f03, f04};

void generate ( std::function<struct position ( float, float, float )> f, int idx = 0 );

void key_callback ( GLFWwindow *window, int key, int scancode, int action,
					int mods ) {
	printf ( "key %i scancode %i action %i mods %i\n", key, scancode, action,
			 mods );
	const char *str = glfwGetKeyName ( key, scancode );
	printf ( "glfw localised name: %s\n", str );
	
	if ( action == GLFW_PRESS ) {
		//decrease samples
		if ( key == '-' ) {
			graph_samples -= 2;
			
			if ( graph_samples == 0 ) graph_samples = 1; //limit to minimum 1
			
			for ( int i = 0; i < N; i++ )
				generate ( funcs[i], i );
				
			//increase samples (keep going to crash)
		}
		else
			if ( key == '=' ) {
				graph_samples += 2;
				
				for ( int i = 0; i < N; i++ )
					generate ( funcs[i], i );
					
				//reset everything
			}
			
		if ( key == 'U' ) {
		
			auto_rotate = !auto_rotate;
			printf ( "auto rotate %d\n", auto_rotate );
		}
		
		else
			if ( key == 'R' ) {
				xang = 0;
				yang = 1.05;
				zpos = -40;
				xpos = 0;
				ypos = 0;
				draw_2d = 1;
				draw_3d = 1;
				graph_samples = 30;
				
				for ( int i = 0; i < N; i++ )
					generate ( funcs[i], i );
					
				//2d only
			}
			else
				if ( key == '2' ) {
					//set perfect position
					xang = 0;
					yang = 0;
					zpos = -20;
					xpos = 0;
					ypos = -20;
					draw_2d = 1;
					draw_3d = 0;
					//3d only
				}
				else
					if ( key == '3' ) {
						draw_3d = 1;
						draw_2d = 0;
						//both 2d + 3d
					}
					else
						if ( key == '1' ) {
							draw_3d = 1;
							draw_2d = 1;
							//neither, axis only
						}
						else
							if ( key == '0' ) {
								draw_3d = 0;
								draw_2d = 0;
							}
	}
	
}

double last_time = glfwGetTime();
int num_frames = 0;
float FPS = 0.;

void update_FPS ( void ) {

	double current_time = glfwGetTime();
	num_frames++;
	
	double t = current_time - last_time;
	
	char title [256];
	title [255] = '\0';
	
	snprintf ( title, 255,
			   "%06.3f s, %04d, FPS: %04.1f\n", current_time, num_frames, FPS );
			   
	// printf ( "current_time: %6.3f, Frame: %03d, FPS: %.1f\n", current_time, num_frames, FPS );
	glfwSetWindowTitle ( window, title );
	
	if ( t >= 1.0 ) {
	
		FPS = num_frames / t;
		num_frames = 0;
		last_time += 1.0;
		
	}
}

//generate graph data function
void generate ( std::function <struct position ( float, float, float ) > fun, int idx ) {

	//reset data
	int estimatedPolys = pow ( graph_samples * 2, 2 );
	struct position graph_3d_1[estimatedPolys];
	struct position graph_3d_2[estimatedPolys];
	//2d graph
	estimatedPolys = graph_samples * 2;
	struct position graph_2d[estimatedPolys];
	
	//ints for point generators
	int i, j;
	int index = 0;
	
	//generate graph_3d_1
	for ( i = -graph_samples; i < graph_samples; i++ ) {
		for ( j = -graph_samples; j < graph_samples; j++ ) {
		
			graph_3d_1[index] = fun ( ( float ) i, ( float ) j, graph_y_scale );
			
			index++;
		}
	}
	
	//generate graph_3d_2
	index = 0;
	
	for ( i = -graph_samples; i < graph_samples; i++ ) {
		for ( j = -graph_samples; j < graph_samples; j++ ) {
		
			graph_3d_2[index] = fun ( ( float ) j, ( float ) i, graph_y_scale );
			
			index++;
		}
	}
	
	//generate graph_2d
	for ( i = -graph_samples; i < graph_samples; ++i ) {
		index = i + graph_samples;
		//2d - z always 1
		graph_2d[index].z = 0;
		//x always same
		graph_2d[index].x = i;
		
		if ( i == 0 ) graph_2d[index].y = graph_y_scale;
		else
			graph_2d[index] = fun ( ( float ) i, ( float ) 0, graph_y_scale );
	}
	
	
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_3d_1[idx] );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( graph_3d_1 ), graph_3d_1, GL_STATIC_DRAW );
	
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_3d_2[idx] );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( graph_3d_2 ), graph_3d_2, GL_STATIC_DRAW );
	
	//buffer for 2d graph
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_2d[idx] );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( graph_2d ), graph_2d, GL_STATIC_DRAW );
	
	
	//console debug
	printf ( "3D graph generated, samples used: %d\n", graph_samples * 2 );
	printf ( "2D graph generated, samples used: %d\n", graph_samples * 2 );
	int polyCount = ( pow ( graph_samples * 2, 2 ) + graph_samples * 2 );
	printf ( "Total number of polygons: %d\n", polyCount );
}

//draw function
void draw ( int idx = 0 ) {
	int i;
	
	//use our shader program
	glUseProgram ( program );
	
	//setup offset + angle for everything
	glUniform3f ( uniform_offset, xpos, ypos, zpos );
	glUniform2f ( uniform_angle, xang, yang );
	
	//color for axis (green)
	glUniform4f ( uniform_color, 0, 1, 0, 1 );
	//draw axis
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_axis );
	glEnableVertexAttribArray ( 0 );
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glDrawArrays ( GL_LINES, 0, 9 );
	glDisableVertexAttribArray ( 0 );
	
	//axis marks
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_axis_marks );
	glEnableVertexAttribArray ( 0 );
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glDrawArrays ( GL_LINES, 0, 600 );
	glDisableVertexAttribArray ( 0 );
	
	//allowed to draw?
	if ( draw_3d ) {
		//color for 3d graph pt 1
		glUniform4f ( uniform_color, 1, 0.5, 0.1, 1 );
		// graph
		glBindBuffer ( GL_ARRAY_BUFFER, buffer_3d_1[idx] );
		glEnableVertexAttribArray ( 0 );
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		
		for ( i = 0; i < graph_samples * 2; i++ )
			glDrawArrays ( GL_LINE_STRIP, i * graph_samples * 2, graph_samples * 2 );
			
		glDisableVertexAttribArray ( 0 );
		//color for 3d graph pt 2
		glUniform4f ( uniform_color, 1, 0, 0, 1 );
		glBindBuffer ( GL_ARRAY_BUFFER, buffer_3d_2[idx] );
		glEnableVertexAttribArray ( 0 );
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		
		for ( i = 0; i < graph_samples * 2; i++ )
			glDrawArrays ( GL_LINE_STRIP, i * graph_samples * 2, graph_samples * 2 );
			
		glDisableVertexAttribArray ( 0 );
	}
	
	//allowed to draw?
	if ( draw_2d ) {
		//color for 2d graph
		glUniform4f ( uniform_color, 1, 0, 1, 1 );
		glBindBuffer ( GL_ARRAY_BUFFER, buffer_2d[idx] );
		glEnableVertexAttribArray ( 0 );
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		glDrawArrays ( GL_LINE_STRIP, 0, graph_samples * 4 );
		glDisableVertexAttribArray ( 0 );
	}
	
	//stop using latest buffer + shader
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glUseProgram ( 0 );
}

int init() {

	if ( !glfwInit () ) {
		fprintf ( stderr, "ERROR: could not start GLFW3\n" );
		return 1;
	}
	
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint ( GLFW_STENCIL_BITS, 8 );
	glfwWindowHint ( GLFW_SAMPLES, 8 );
	
	
	const GLFWvidmode *mode = glfwGetVideoMode ( glfwGetPrimaryMonitor() );
	
	//use 2/3 of the screen by default if window
	if ( !fullscreen ) {
		width = 2 * mode->width / 3;
		height = 2 * mode->height / 3;
	}
	else {
	
		width = mode->width;
		height = mode->height;
	}
	
	window = glfwCreateWindow ( width, height, "Graph", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr );
	
	glfwGetFramebufferSize ( window, &width, &height );
	glViewport ( 0, 0, width, height );
	
	if ( !window ) {
		fprintf ( stderr, "ERROR: could not open window with GLFW3\n" );
		glfwTerminate();
		return 1;
	}
	
	glfwMakeContextCurrent ( window );
	glfwSetWindowSizeCallback ( window, window_size_callback );
	glfwSetKeyCallback ( window, key_callback );
	
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	
	if ( GLEW_OK != err ) {
		/* Problem: glewInit failed, something is seriously wrong. */
		printf ( "Problem: glewInit failed, something is seriously wrong.\n" );
		return 0;
	}
	
	
	// get version info
	const GLubyte *renderer = glGetString ( GL_RENDERER ); // get renderer string
	const GLubyte *version = glGetString ( GL_VERSION ); // version as a string
	
	printf ( "Renderer: %s\n", renderer );
	printf ( "OpenGL version supported %s\n", version );
	
	glEnable ( GL_DEPTH_TEST ); // enable depth-testing
	glDepthFunc ( GL_LESS ); // depth-testing interprets a smaller value as "closer"
	glClearColor ( 0.5f, 0.5f, 0.5f, 0.5f );
	//glEnable(GL_CULL_FACE);
	glCullFace ( GL_BACK );
	glFrontFace ( GL_CCW );
	
	// glLineWidth(5.0f);		// Set a 'chunky' line width
	// glEnable(GL_LINE_SMOOTH);	// Enable anti-aliasing on lines
	
	// glPointSize(5.0f);		// Set a 'chunky' point size
	// glEnable(GL_POINT_SMOOTH);	// Enable anti-aliasing on points
	
	// glEnable ( GL_LINE_SMOOTH );
	glEnable ( GL_POLYGON_SMOOTH );
	glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint ( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	
	glClearColor ( 0.4, 0.4, 0.4, 0.5 );
	
	/* shaders */
	
	//compile vertex shader
	GLuint v_shader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( v_shader, 1, ( const GLchar ** ) &vertex_shader, 0 );
	glCompileShader ( v_shader );
	//compile frag shader
	GLuint f_shader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( f_shader, 1, ( const GLchar ** ) &fragment_shader, 0 );
	glCompileShader ( f_shader );
	//create the shader program
	program = glCreateProgram();
	glAttachShader ( program, v_shader );
	glAttachShader ( program, f_shader );
	glLinkProgram ( program );
	
	//get uniform locations in shaders
	uniform_offset = glGetUniformLocation ( program, "offset" );
	uniform_angle = glGetUniformLocation ( program, "angle" );
	uniform_color = glGetUniformLocation ( program, "color" );
	uniform_perspective = glGetUniformLocation ( program, "perspective" );
	
	
	/* buffers */
	
	//buffers for 3d graph
	glGenBuffers ( N, buffer_3d_1 );
	glGenBuffers ( N, buffer_3d_2 );
	glGenBuffers ( N, buffer_2d );
	
	/* VAO */
	glGenVertexArrays ( 1, &vao );
	glBindVertexArray ( vao );
	
	//setup perspective
	float fFrustumScale = 1.0f;
	float fzNear = 1.0f;
	float fzFar = 100.0f;
	//THE MATRIX
	float theMatrix[16];
	memset ( theMatrix, 0, sizeof ( float ) * 16 );
	//build perspective matrix
	theMatrix[0] = fFrustumScale;
	theMatrix[5] = fFrustumScale;
	theMatrix[10] = ( fzFar + fzNear ) / ( fzNear - fzFar );
	theMatrix[14] = ( 2 * fzFar * fzNear ) / ( fzNear - fzFar );
	theMatrix[11] = -1.0f;
	//bind data to shader
	glUseProgram ( program );
	glUniformMatrix4fv ( uniform_perspective, 1, GL_TRUE, theMatrix ); //perspective matrix
	glUniform3f ( uniform_offset, xpos, ypos, zpos ); //position offset
	glUniform2f ( uniform_angle, xang, yang ); //rotation angles
	glUniform4f ( uniform_color, 1, 1, 1, 1 ); //color
	glUseProgram ( 0 );
	
	//axis lines
	//x
	axis[0].x = -graph_size;
	axis[0].y = 0;
	axis[0].z = 0;
	axis[1].x = graph_size;
	axis[1].y = 0;
	axis[1].z = 0;
	//y
	axis[2].x = 0;
	axis[2].y = -graph_size;
	axis[2].z = 0;
	axis[3].x = 0;
	axis[3].y = graph_size;
	axis[3].z = 0;
	//z
	axis[4].x = 0;
	axis[4].y = 0;
	axis[4].z = graph_size;
	axis[5].x = 0;
	axis[5].y = 0;
	axis[5].z = -graph_size;
	
	//buffer for axis
	glGenBuffers ( 1, &buffer_axis );
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_axis );
	//assign axis data
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( axis ), axis, GL_STATIC_DRAW );
	
	//axis marks
	int i;
	int index = 0;
	
	//x
	for ( i = -graph_size; i < graph_size; ++i ) {
		if ( i % 2 != 0 ) continue; //only every 2 points
		
		axis_marks[index].x = i;
		axis_marks[index].y = 0;
		axis_marks[index].z = 0;
		index++;
		axis_marks[index].x = i;
		axis_marks[index].y = 1;
		axis_marks[index].z = 0;
		index++;
	}
	
	//y
	for ( i = -graph_size; i < graph_size; ++i ) {
		if ( i % 2 != 0 ) continue;
		
		axis_marks[index].x = 0;
		axis_marks[index].y = i;
		axis_marks[index].z = 0;
		index++;
		axis_marks[index].x = 1;
		axis_marks[index].y = i;
		axis_marks[index].z = 0;
		index++;
	}
	
	//z
	for ( i = -graph_size; i < graph_size; ++i ) {
		if ( i % 2 != 0 ) continue;
		
		axis_marks[index].x = 0;
		axis_marks[index].y = 0;
		axis_marks[index].z = i;
		index++;
		axis_marks[index].x = 0;
		axis_marks[index].y = 1;
		axis_marks[index].z = i;
		index++;
	}
	
	//buffer for axis marks
	glGenBuffers ( 1, &buffer_axis_marks );
	glBindBuffer ( GL_ARRAY_BUFFER, buffer_axis_marks );
	//assign axis_marks data
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( axis_marks ), axis_marks, GL_STATIC_DRAW );
	
	
}

void keyboard() {
	//keyboard management
	if ( glfwGetKey ( window, 'A' ) == GLFW_PRESS )
		xpos = xpos - 0.1;
		
	if ( glfwGetKey ( window, 'D' ) == GLFW_PRESS )
		xpos = xpos + 0.1;
		
	if ( glfwGetKey ( window, 'W' ) == GLFW_PRESS )
		ypos = ypos + 0.1;
		
	if ( glfwGetKey ( window, 'S' ) == GLFW_PRESS )
		ypos = ypos - 0.1;
		
	if ( glfwGetKey ( window, 'E' ) == GLFW_PRESS )
		zpos = zpos - 0.1;
		
	if ( glfwGetKey ( window, 'Q' ) == GLFW_PRESS )
		zpos = zpos + 0.1;
		
	if ( glfwGetKey ( window, GLFW_KEY_LEFT ) == GLFW_PRESS )
		xang = xang - 0.025;
		
	if ( glfwGetKey ( window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
		xang = xang + 0.025;
		
	if ( glfwGetKey ( window, GLFW_KEY_UP ) == GLFW_PRESS )
		yang = yang - 0.025;
		
	if ( glfwGetKey ( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
		yang = yang + 0.025;
		
	if ( glfwGetKey ( window, GLFW_KEY_F11 ) ) {
	
		// Toggle fullscreen flag.
		fullscreen = !fullscreen;
		
		// Close the current window.
		// glfwCloseWindow();
		
		// Renew calls to glfwOpenWindowHint.
		// (Hints get reset after the call to glfwOpenWindow.)
		// myGLFWOpenWindowHints();
		
		// Create the new window.
		// new_window = glfwCreateWindow ( width, height, "Graph", NULL, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW );
		
		
		// https://www.gamedev.net/topic/672308-glfw-toggle-fullscreen-to-windowed-to-fullscreen/
		
	}
	
	// printf ( "%f %f %f %f %f\n", xpos, ypos, zpos, xang, yang );
	
}

int main ( void ) {

	init();
	
	for ( int i = 0; i < N; i++ )
		generate ( funcs[i], i );
		
	/* main loop */
	while ( !glfwWindowShouldClose ( window ) ) {
	
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glViewport ( 0, 0, width / 2, height / 2 );
		draw ( 2 );
		
		glViewport ( width / 2, 0, width / 2, height / 2 );
		draw ( 3 );
		
		glViewport ( 0, height / 2, width / 2, height / 2 );
		draw ( 0 );
		
		glViewport ( width / 2, height / 2, width / 2, height / 2 );
		draw ( 1 );
		
		glfwSwapBuffers ( window );
		glfwPollEvents ();
		
		if ( auto_rotate )
			xang -= 0.005f;
			
		update_FPS();
		
		//TODO: Use two threads, one for polling events and one for rendering. You also might want to use glfwWaitEvents intstead of glfwPollEvents to block the event thread until events are available.
		
		if ( GLFW_PRESS == glfwGetKey ( window, GLFW_KEY_ESCAPE ) )
			glfwSetWindowShouldClose ( window, 1 );
			
		keyboard();
		
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
	
}
