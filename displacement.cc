/*
* @Author: Kamil Rocki
* @Date:   2017-02-28 11:25:34
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-02-28 14:11:55
*/

#include <iostream>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/theme.h>
#include <nanogui/formhelper.h>
#include <nanogui/slider.h>

using namespace std;
using nanogui::Screen;
using nanogui::Window;
using nanogui::GroupLayout;
using nanogui::Button;
using nanogui::Vector2f;
using nanogui::MatrixXu;
using nanogui::MatrixXf;
using nanogui::Label;

char *file_read ( const char *filename, size_t *sz ) {
	FILE *in = fopen ( filename, "rb" );
	
	if ( in == NULL ) {
	
		printf ( "Couldn't read %s !\n", filename );
		
		return NULL;
		
	}
	
	int res_size = BUFSIZ;
	char *res = ( char * ) malloc ( res_size );
	int nb_read_total = 0;
	
	while ( !feof ( in ) && !ferror ( in ) ) {
		if ( nb_read_total + BUFSIZ > res_size ) {
			if ( res_size > 10 * 1024 * 1024 ) break;
			
			res_size = res_size * 2;
			res = ( char * ) realloc ( res, res_size );
		}
		
		char *p_res = res + nb_read_total;
		nb_read_total += fread ( p_res, 1, BUFSIZ, in );
	}
	
	fclose ( in );
	res = ( char * ) realloc ( res, nb_read_total + 1 );
	res[nb_read_total] = '\0';
	
	*sz = nb_read_total;
	
	printf ( "Read %s (%zu Bytes)\n", filename, *sz );
	return res;
}

int main() {

	nanogui::init();
	
	/**
	 * Create a screen, add a window.
	 * To the window add a label and a slider widget.
	 */
	
	Screen app{{1024 / 2, 768 / 2}, "NanoGUI Test"};
	
	Window window{&app, ""};
	window.setPosition ( {15, 15} );
	window.setLayout ( new GroupLayout ( 5, 5, 0, 0 ) );
	
	Label *l = new Label ( &window, "MODULATION", "sans-bold" );
	l->setFontSize ( 10 );
	nanogui::Slider *slider = new nanogui::Slider ( &window );
	slider->setValue ( 0.5f );
	float modulation = 5.0f;
	slider->setCallback ( [&modulation] ( float value ) { modulation = value * 10.0f; } );
	
	// Do the layout calculations based on what was added to the GUI
	app.performLayout();
	
	
	/**
	 * Load GLSL shader code from embedded resources
	 * See: https://github.com/cyrilcode/embed-resource
	 */
	nanogui::GLShader mShader;
	size_t sz_vertShader, sz_fragShader;
	char *vertShader = file_read ( "disp_vert.glsl", &sz_vertShader );
	char *fragShader = file_read ( "disp_frag.glsl", &sz_fragShader );
	
	
	// 			mShader.init (
	// 			/* An identifying name */
	// 			"a_simple_shader",
	
	// 			/* Vertex shader */
	// 			"#version 330\n"
	// 			"uniform mat4 modelViewProj;\n"
	// 			"in vec3 position;\n"
	// 			"void main() {\n"
	// 			"    gl_Position = modelViewProj * vec4(position, 1.0);\n"
	// 			"}",
	
	// 			/* Fragment shader */
	// 			"#version 330\n"
	// 			"out vec4 color;\n"
	// 			"uniform float intensity;\n"
	// 			"void main() {\n"
	// 			"    color = vec4(vec3(intensity), 1.0);\n"
	// 			"}"
	// 		);
	
	mShader.init ( "raymarching_shader",
				   string ( vertShader, sz_vertShader ),
				   string ( fragShader, sz_fragShader )
				 );
				 
	/**
	 * Fill the screen with a rectangle (2 triangles)
	 */
	MatrixXu indices ( 3, 2 );
	indices.col ( 0 ) << 0, 1, 2;
	indices.col ( 1 ) << 2, 1, 3;
	MatrixXf positions ( 3, 4 );
	positions.col ( 0 ) << -1, -1, 0;
	positions.col ( 1 ) <<  1, -1, 0;
	positions.col ( 2 ) <<  -1,  1, 0;
	positions.col ( 3 ) << 1,  1, 0;
	// bind the shader and upload vertex positions and indices
	mShader.bind();
	mShader.uploadIndices ( indices );
	mShader.uploadAttrib ( "a_position", positions );
	
	// Set initial value for modulation uniform
	mShader.setUniform ( "time", modulation );
	
	// Set resolution and screenRatio uniforms
	int fboWidth, fboHeight;
	glfwGetFramebufferSize ( app.glfwWindow(), &fboWidth, &fboHeight );
	mShader.setUniform ( "resolution", Vector2f{fboWidth, fboHeight} );
	float mx = std::max<float> ( fboWidth, fboHeight );
	auto xDim = fboWidth / mx;
	auto yDim = fboHeight / mx;
	mShader.setUniform ( "screenRatio", Vector2f{xDim, yDim} );
	
	app.drawAll();
	app.setVisible ( true );
	
	/**
	 * 10: clear screen
	 * 20: set modulation value
	 * 30: draw using shader
	 * 40: draw GUI
	 * 50: goto 10
	 */
	while ( !glfwWindowShouldClose ( app.glfwWindow() ) ) {
		glClearColor ( 0, 0, 0, 1 );
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		mShader.bind();
		mShader.setUniform ( "time", modulation );
		
		mShader.drawIndexed ( GL_TRIANGLES, 0, 2 );
		
		app.drawWidgets();
		
		glfwSwapBuffers ( app.glfwWindow() );
		glfwPollEvents();
	}
	
	nanogui::shutdown();
	exit ( EXIT_SUCCESS );
}
