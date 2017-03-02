/*
* @Author: Kamil Rocki
* @Date:   2017-02-28 11:25:34
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-02 09:59:52
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <cstdio>

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/theme.h>
#include <nanogui/formhelper.h>
#include <nanogui/slider.h>
#include <nanogui/graph.h>
#include <nanogui/sq_graph.h>
#include <nanogui/console.h>

#include "paragraph.h"

using namespace std;
using nanogui::Screen;
using nanogui::Window;
using nanogui::GroupLayout;
using nanogui::Button;
using nanogui::Vector2f;
using nanogui::MatrixXu;
using nanogui::MatrixXf;
using nanogui::Label;

#define MAX_BUF 4096

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

double last_time = glfwGetTime();
int num_frames = 0;
#define HISTORY_SIZE 50
std::vector<float> FPS ( HISTORY_SIZE, 0 );

void update_FPS ( void ) {

	double current_time = glfwGetTime();
	num_frames++;
	
	double t = current_time - last_time;
	
	float interval = 0.25f;
	
	if ( t >= interval ) {
	
		FPS.push_back ( ( num_frames / t ) );
		FPS.erase ( FPS.begin() );
		
		num_frames = 0;
		last_time += interval;
		
	}
}

class DisplacementMap : public nanogui::Screen {

	public:
		DisplacementMap ( bool fullscreen = false, int aliasing_samples = 8 ) :
			nanogui::Screen ( Eigen::Vector2i ( 1024, 768 ),
							  "NanoGUI Test", true, fullscreen, 8, 8, 24, 8, aliasing_samples,
							  3, 3 ) {
							  
			load_shaders();
			init();
			
		}
		
		void load_shaders() {
		
			size_t sz_vertShader, sz_fragShader;
			char *vertShader = file_read ( "disp_vert.glsl", &sz_vertShader );
			char *fragShader = file_read ( "disp2_frag.glsl", &sz_fragShader );
			
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
			
			// this->resizeEvent ( {mFBSize[0], mFBSize[1]} );
			mShader.setUniform ( "resolution", Vector2f{mFBSize[0], mFBSize[1]} );
			
			
		}
		
		void console ( const char *pMsg, ... ) {
		
			char buffer[4096];
			std::va_list arg;
			va_start ( arg, pMsg );
			std::vsnprintf ( buffer, 4096, pMsg, arg );
			va_end ( arg );
			log_str.append ( buffer );
			
		}
		
		void init() {
		
			int glfw_window_width, glfw_window_height;
			glfwGetWindowSize ( glfwWindow(), &glfw_window_width, &glfw_window_height );
			
			// get version info
			const GLubyte *renderer = glGetString ( GL_RENDERER ); // get renderer string
			const GLubyte *version = glGetString ( GL_VERSION ); // version as a string
			
			console ( "GL_RENDERER: %s\n", renderer );
			console ( "GL_VERSION: %s\n\n", version );
			console ( "glfwGetWindowSize(): %d x %d\n", glfw_window_width, glfw_window_height );
			
			Window *window = new Window ( this, "" );
			window->setPosition ( {15, 15} );
			nanogui::GridLayout *layout =
				new nanogui::GridLayout ( nanogui::Orientation::Horizontal, 3,
										  nanogui::Alignment::Middle, 15, 5 );
			layout->setColAlignment (
			{ nanogui::Alignment::Maximum, nanogui::Alignment::Fill } );
			layout->setSpacing ( 0, 10 );
			window->setLayout ( layout );
			
			
			nanogui::Theme *t = window->theme();
			t->mWindowFillUnfocused = nanogui::Color ( 128, 128, 128, 64 );
			window->setTheme ( t );
			
			Label *l = new Label ( window, "MODULATION", "sans-bold" );
			l->setFontSize ( 10 );
			slider = new nanogui::Slider ( window );
			slider->setValue ( 0.5f );
			modulation = 5.0f;
			slider->setCallback ( [this] ( float value ) { this->modulation = value * 10.0f; } );
			auto_change_box = new nanogui::CheckBox ( window, "auto" );
			auto_change_box->setCallback ( [this] ( float value ) {
				auto_change = !auto_change; console ( "auto_change: %d\n", auto_change );
			} );
			
			int graph_width = 110;
			int graph_height = 15;
			graphDyn = add<nanogui::Graph> ( "" );
			graphDyn->setPosition ( {5, glfw_window_height - graph_height - 5} );
			graphDyn->setSize ( {graph_width, graph_height } );
			graphDyn->values().resize ( HISTORY_SIZE );
			graphDyn->setGraphColor ( nanogui::Color ( 0, 160, 192, 255 ) );
			// graphDyn->setFill ( true );
			
			/* console */
			int console_width = 350;
			int console_height = mFBSize[1] - 10;
			show_console = false;
			auto_change = true;
			
			window_test = new Window ( this, "" );
			window_test->setPosition ( {glfw_window_width - console_width - 5, 5} );
			window_test->setSize ( {console_width, console_height} );
			window_test->setVisible ( show_console );
			
			// window_test->setLayout ( new GroupLayout ( 5, 5, 0, 0 ) );
			console_test = new nanogui::Console ( window_test );
			console_test->setPosition ( {5, 5} );
			console_test->setWidth ( console_width - 10 );
			console_test->setHeight ( console_height - 10 );
			console_test->setFontSize ( 12 );
			
			performLayout();
			
		}
		
		~DisplacementMap() {
			mShader.free();
		}
		
		virtual bool keyboardEvent ( int key, int scancode, int action, int modifiers ) {
			if ( Screen::keyboardEvent ( key, scancode, action, modifiers ) )
				return true;
				
			if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
				setVisible ( false );
				return true;
			}
			
			if ( key == GLFW_KEY_F && action == GLFW_PRESS ) {
			
				// TODO
				// printf ( "Fullscreen = %d\n", fullscreen );
				
				// GLFWwindow *old_window = this->mGLFWWindow;
				// GLFWwindow *new_window = glfwCreateWindow ( mFBSize[0], mFBSize[1], "", fullscreen ? glfwGetPrimaryMonitor() : NULL,
				// 						 NULL );
				// this->mGLFWWindow = new_window;
				// glfwMakeContextCurrent ( this->mGLFWWindow );
				// glfwDestroyWindow ( old_window );
				
			}
			
			if ( key == GLFW_KEY_TAB && action == GLFW_PRESS ) {
			
				show_console = !show_console;
				window_test->setVisible ( show_console );
				
			}
			
			return false;
		}
		
		virtual void draw ( NVGcontext *ctx ) {
		
			/* Animate the scrollbar */
			if ( auto_change ) {
				slider->setValue ( std::fmod ( ( float ) glfwGetTime() / 20, 1.0f ) );
				slider->callback() ( slider->value() );
			}
			
			graphDyn->values() = Eigen::Map<Eigen::VectorXf> ( FPS.data(), HISTORY_SIZE );
			
			char str[16];
			int last_avg = 10;
			sprintf ( str, "%3.1f FPS\n", graphDyn->values().block ( HISTORY_SIZE - 1 - last_avg, 0, last_avg, 1 ).mean() );
			
			// graphDyn->setHeader ( str );
			graphDyn->setHeader ( str );
			
			console_test->setValue ( log_str );
			
			/* Draw the user interface */
			Screen::draw ( ctx );
			update_FPS();
			
			float mx = 1;
			float my = 1;
			
		}
		
		virtual void drawContents() {
			using namespace nanogui;
			
			/* Draw the window contents using OpenGL */
			mShader.bind();
			mShader.setUniform ( "time", modulation );
			mShader.setUniform ( "resolution", Vector2f{mFBSize[0], mFBSize[1]} );
			float mx = std::max<float> ( mFBSize[0], mFBSize[1] );
			auto xDim = mFBSize[1] / mx;
			auto yDim = mFBSize[0] / mx;
			this->mShader.setUniform ( "screenRatio", Vector2f{xDim, yDim} );
			
			mShader.drawIndexed ( GL_TRIANGLES, 0, 2 );
			
		}
		
		nanogui::Label *l;
		nanogui::Slider *slider;
		nanogui::GLShader mShader;
		nanogui::Graph *graphDyn;
		nanogui::Window *window_test;
		nanogui::Console *console_test;
		nanogui::CheckBox *auto_change_box;
		std::string log_str;
		float modulation;
		
		bool show_console;
		bool auto_change;
		
};


int main ( int /* argc */, char ** /* argv */ ) {
	try {
		nanogui::init();
		
		/* scoped variables */ {
			nanogui::ref<DisplacementMap> app = new DisplacementMap();
			app->drawAll();
			app->setVisible ( true );
			nanogui::mainloop ( 1 );
		}
		
		nanogui::shutdown();
	}
	catch ( const std::runtime_error &e ) {
		std::string error_msg = std::string ( "Caught a fatal error: " ) + std::string ( e.what() );
		#if defined(_WIN32)
		MessageBoxA ( nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK );
		#else
		std::cerr << error_msg << endl;
		#endif
		return -1;
	}
	
	return 0;
}
