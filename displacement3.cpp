/*
* @Author: Kamil Rocki
* @Date:   2017-02-28 11:25:34
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-02 11:41:01
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
#include <nanogui/glcanvas.h>

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

class MyGLCanvas : public nanogui::GLCanvas {

	public:
		MyGLCanvas ( Widget *parent ) : nanogui::GLCanvas ( parent ), mRotation ( nanogui::Vector3f ( 0.25, 0.5, 0.33 ) ) {
		
		
			size_t sz_vertShader, sz_fragShader;
			char *vertShader = file_read ( "vert_w.glsl", &sz_vertShader );
			char *fragShader = file_read ( "frag_w.glsl", &sz_fragShader );
			
			mShader.init ( "cube_shader",
						   string ( vertShader, sz_vertShader ),
						   string ( fragShader, sz_fragShader )
						 );
						 
			MatrixXu indices ( 3, 12 ); /* Draw a cube */
			indices.col ( 0 ) << 0, 1, 3;
			indices.col ( 1 ) << 3, 2, 1;
			indices.col ( 2 ) << 3, 2, 6;
			indices.col ( 3 ) << 6, 7, 3;
			indices.col ( 4 ) << 7, 6, 5;
			indices.col ( 5 ) << 5, 4, 7;
			indices.col ( 6 ) << 4, 5, 1;
			indices.col ( 7 ) << 1, 0, 4;
			indices.col ( 8 ) << 4, 0, 3;
			indices.col ( 9 ) << 3, 7, 4;
			indices.col ( 10 ) << 5, 6, 2;
			indices.col ( 11 ) << 2, 1, 5;
			
			MatrixXf positions ( 3, 8 );
			positions.col ( 0 ) << -1,  1,  1;
			positions.col ( 1 ) << -1,  1, -1;
			positions.col ( 2 ) <<  1,  1, -1;
			positions.col ( 3 ) <<  1,  1,  1;
			positions.col ( 4 ) << -1, -1,  1;
			positions.col ( 5 ) << -1, -1, -1;
			positions.col ( 6 ) <<  1, -1, -1;
			positions.col ( 7 ) <<  1, -1,  1;
			
			MatrixXf colors ( 3, 12 );
			colors.col ( 0 ) << 1, 0, 0;
			colors.col ( 1 ) << 0, 1, 0;
			colors.col ( 2 ) << 1, 1, 0;
			colors.col ( 3 ) << 0, 0, 1;
			colors.col ( 4 ) << 1, 0, 1;
			colors.col ( 5 ) << 0, 1, 1;
			colors.col ( 6 ) << 1, 1, 1;
			colors.col ( 7 ) << 0.5, 0.5, 0.5;
			colors.col ( 8 ) << 1, 0, 0.5;
			colors.col ( 9 ) << 1, 0.5, 0;
			colors.col ( 10 ) << 0.5, 1, 0;
			colors.col ( 11 ) << 0.5, 1, 0.5;
			
			mShader.bind();
			mShader.uploadIndices ( indices );
			
			mShader.uploadAttrib ( "position", positions );
			mShader.uploadAttrib ( "color", colors );
			
			
		}
		
		~MyGLCanvas() {
		
			mShader.free();
			
		}
		
		void setRotation ( nanogui::Vector3f vRotation ) {
			mRotation = vRotation;
		}
		
		virtual void drawGL() override {
		
			using namespace nanogui;
			
			mShader.bind();
			
			Matrix4f mvp;
			mvp.setIdentity();
			float fTime = ( float ) glfwGetTime();
			mvp.topLeftCorner<3, 3>() = Eigen::Matrix3f ( Eigen::AngleAxisf ( mRotation[0] * fTime, Vector3f::UnitX() ) *
										Eigen::AngleAxisf ( mRotation[1] * fTime,  Vector3f::UnitY() ) *
										Eigen::AngleAxisf ( mRotation[2] * fTime, Vector3f::UnitZ() ) ) * 0.25f;
										
			mShader.setUniform ( "modelViewProj", mvp );
			
			glEnable ( GL_DEPTH_TEST );
			/* Draw 12 triangles starting at index 0 */
			mShader.drawIndexed ( GL_TRIANGLES, 0, 12 );
			glDisable ( GL_DEPTH_TEST );
			
		}
		
		nanogui::GLShader mShader;
		Eigen::Vector3f mRotation;
		float modulation;
		
};

class DisplacementMap : public nanogui::Screen {

	public:
		DisplacementMap ( bool fullscreen = false, int aliasing_samples = 8 ) :
			nanogui::Screen ( Eigen::Vector2i ( 1024, 768 ),
							  "NanoGUI Test", true, fullscreen, 8, 8, 24, 8, aliasing_samples,
							  3, 3 ) {
							  
			init();
			
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
			
			// upper left window
			// Window *window = new Window ( this, "" );
			// window->setPosition ( {15, 15} );
			// nanogui::GridLayout *layout =
			// 	new nanogui::GridLayout ( nanogui::Orientation::Horizontal, 3,
			// 							  nanogui::Alignment::Middle, 15, 5 );
			// layout->setColAlignment ( { nanogui::Alignment::Maximum, nanogui::Alignment::Fill } );
			// layout->setSpacing ( 0, 10 );
			// window->setLayout ( layout );
			
			nanogui::Theme *t = this->theme();
			t->mWindowFillUnfocused = nanogui::Color ( 128, 128, 128, 64 );
			this->setTheme ( t );
			
			//GL Canvas # 1
			glwindow = new Window ( this, "" );
			glwindow->setPosition ( {100, 100} );
			glwindow->setSize ( {100, 100} );
			canvas = new MyGLCanvas ( glwindow );
			canvas->setBackgroundColor ( {100, 100, 100, 255} );
			canvas->setPosition ( {5, 5} );
			canvas->setSize ( {90, 90} );
			
			//GL Canvas #2
			glwindow2 = new Window ( this, "" );
			glwindow2->setPosition ( {400, 100} );
			glwindow2->setSize ( {300, 300} );
			canvas2 = new MyGLCanvas ( glwindow2 );
			canvas2->mRotation = Eigen::Vector3f ( 0.0, -0.1, -0.1 );
			canvas2->setBackgroundColor ( {100, 100, 100, 255} );
			canvas2->setPosition ( {5, 5} );
			canvas2->setSize ( {290, 290} );
			
			// slider
			// Label *l = new Label ( window, "MODULATION", "sans-bold" );
			// l->setFontSize ( 10 );
			// slider = new nanogui::Slider ( window );
			// slider->setValue ( 0.5f );
			// canvas->modulation = 5.0f;
			// slider->setCallback ( [this] ( float value ) { this->canvas->modulation = value * 10.0f; } );
			
			// //checkbox
			// auto_change_box = new nanogui::CheckBox ( window, "auto" );
			// auto_change_box->setCallback ( [this] ( float value ) {
			// 	auto_change = !auto_change; console ( "auto_change: %d\n", auto_change );
			// } );
			
			//bottom left graph
			graphDyn = add<nanogui::Graph> ( "" );
			graphDyn->values().resize ( HISTORY_SIZE );
			graphDyn->setGraphColor ( nanogui::Color ( 0, 160, 192, 255 ) );
			graphDyn->setBackgroundColor ( nanogui::Color ( 0, 0, 0, 8 ) );
			// graphDyn->setFill ( true );
			
			/* console */
			show_console = false;
			auto_change = false;
			
			window_test = new Window ( this, "" );
			window_test->setVisible ( show_console );
			
			// window_test->setLayout ( new GroupLayout ( 5, 5, 0, 0 ) );
			console_test = new nanogui::Console ( window_test );
			console_test->setFontSize ( 12 );
			
			resizeEvent ( { glfw_window_width, glfw_window_height } );
			
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
			// if ( auto_change ) {
			// 	slider->setValue ( std::fmod ( ( float ) glfwGetTime() / 20, 1.0f ) );
			// 	slider->callback() ( slider->value() );
			// }
			
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
		
		virtual bool resizeEvent ( const Eigen::Vector2i &size ) {
		
			int graph_width = 110;
			int graph_height = 15;
			graphDyn->setPosition ( {5, size[1] - graph_height - 5} );
			graphDyn->setSize ( {graph_width, graph_height } );
			
			int console_width = 350;
			int console_height = size[1] - 10;
			window_test->setPosition ( {size[0] - console_width - 5, 5} );
			window_test->setSize ( {console_width, console_height} );
			
			console_test->setPosition ( {5, 5} );
			console_test->setWidth ( console_width - 10 );
			console_test->setHeight ( console_height - 10 );
			
			performLayout();
			
			return true;
			
		}
		
		// nanogui::Label *l;
		// nanogui::Slider *slider;
		nanogui::Graph *graphDyn;
		nanogui::Window *window_test;
		nanogui::Console *console_test;
		nanogui::CheckBox *auto_change_box;
		MyGLCanvas *canvas;
		Window *glwindow;
		MyGLCanvas *canvas2;
		Window *glwindow2;
		std::string log_str;
		
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
