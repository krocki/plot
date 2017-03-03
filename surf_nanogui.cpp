/*
* @Author: Kamil Rocki
* @Date:   2017-02-28 11:25:34
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-03 10:24:06
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <iomanip>

#include <GLFW/glfw3.h>

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

using namespace std;
using nanogui::Screen;
using nanogui::Window;
using nanogui::GroupLayout;
using nanogui::Button;
using nanogui::Vector2f;
using nanogui::MatrixXu;
using nanogui::MatrixXf;
using nanogui::Label;

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

// * for plotting function
#define N 100000

float rand_float ( float mn, float mx ) {
	float r = random() / ( float ) RAND_MAX;
	return mn + ( mx - mn ) * r;
}

float hat ( float x, float y ) {

	float t = hypotf ( x, y ) * 4.0;
	float z = ( 1 - t * t ) * expf ( t * t / -2.0 );
	return z;
}

float hue2rgb ( float p, float q, float t ) {
	float tt = t;
	if ( tt < 0.0 ) tt += 1.0;
	if ( tt > 1.0 ) tt -= 1.0;
	if ( tt < 1.0 / 6.0 ) return p + ( q - p ) * 6.0 * tt;
	if ( tt < 1.0 / 2.0 ) return q;
	if ( tt < 2.0 / 3.0 ) return p + ( q - p ) * ( 2.0 / 3.0 - tt ) * 6.0;
	return p;
}

Eigen::Vector3f hslToRgb ( float h, float s, float l ) {
	float r, g, b;
	if ( s == 0.0 ) {
		r = g = b = l; // achromatic
	}
	else {
		float q;
		if ( l < 0.5 )
			q = l * ( 1.0 + s );
		else
			q = l + s - l * s;
			
		float p = 2.0 * l - q;
		r = hue2rgb ( p, q, h + 1.0 / 3.0 );
		g = hue2rgb ( p, q, h );
		b = hue2rgb ( p, q, h - 1.0 / 3.0 );
	}
	
	return Eigen::Vector3f ( r, g, b );
}

class DisplacementMap : public nanogui::Screen {

	public:
		DisplacementMap ( bool fullscreen = false, int aliasing_samples = 8 ) :
			nanogui::Screen ( Eigen::Vector2i ( 1024, 768 ), "Surface Plot", true, fullscreen, 8, 8, 24, 8, aliasing_samples, 3,
							  3 ) {
							  
			load_shaders();
			init();
			
		}
		
		void load_shaders() {
		
			mShader.initFromFiles ( "surf3d", "passthrough.v.glsl", "passthrough.f.glsl" );
			
			// FLAT
			/**
			 * Fill the screen with a rectangle (2 triangles)
			 */
			// MatrixXu indices ( 3, 2 );
			// indices.col ( 0 ) << 0, 1, 2;
			// indices.col ( 1 ) << 2, 1, 3;
			
			// MatrixXf positions ( 3, 4 );
			// positions.col ( 0 ) << -1, -1, 0;
			// positions.col ( 1 ) <<  1, -1, 0;
			// positions.col ( 2 ) <<  -1,  1, 0;
			// positions.col ( 3 ) << 1,  1, 0;
			
			// MatrixXf colors ( 4, 4 );
			// colors.col ( 0 ) << 0.0f, 0.5f, 1.0f, 1.0f;
			// colors.col ( 1 ) <<  1.0f, 0.5f, 0.0f, 1.0f;
			// colors.col ( 2 ) <<  0.0f, 1.0f, 0.0f, 1.0f;
			// colors.col ( 3 ) << 1.0f, 0.0f, 1.0f, 1.0f;
			
			/* CUBE */
			// MatrixXu indices ( 3, 12 ); /* Draw a cube */
			// indices.col ( 0 ) << 0, 1, 3;
			// indices.col ( 1 ) << 3, 2, 1;
			// indices.col ( 2 ) << 3, 2, 6;
			// indices.col ( 3 ) << 6, 7, 3;
			// indices.col ( 4 ) << 7, 6, 5;
			// indices.col ( 5 ) << 5, 4, 7;
			// indices.col ( 6 ) << 4, 5, 1;
			// indices.col ( 7 ) << 1, 0, 4;
			// indices.col ( 8 ) << 4, 0, 3;
			// indices.col ( 9 ) << 3, 7, 4;
			// indices.col ( 10 ) << 5, 6, 2;
			// indices.col ( 11 ) << 2, 1, 5;
			
			// MatrixXf positions ( 3, 8 );
			// positions.col ( 0 ) << -1,  1,  1;
			// positions.col ( 1 ) << -1,  1, -1;
			// positions.col ( 2 ) <<  1,  1, -1;
			// positions.col ( 3 ) <<  1,  1,  1;
			// positions.col ( 4 ) << -1, -1,  1;
			// positions.col ( 5 ) << -1, -1, -1;
			// positions.col ( 6 ) <<  1, -1, -1;
			// positions.col ( 7 ) <<  1, -1,  1;
			
			// MatrixXf colors ( 3, 12 );
			// colors.col ( 0 ) << 1, 0, 0;
			// colors.col ( 1 ) << 1, 0, 0;
			// colors.col ( 2 ) << 1, 0, 0;
			// colors.col ( 3 ) << 1, 0, 0;
			// colors.col ( 4 ) << 0, 0, 1;
			// colors.col ( 5 ) << 0, 0, 1;
			// colors.col ( 6 ) << 0, 0, 1;
			// colors.col ( 7 ) << 0, 1, 0;
			// colors.col ( 8 ) << 0, 1, 0;
			// colors.col ( 9 ) << 0, 1, 0;
			// colors.col ( 10 ) << 0, 1, 0;
			// colors.col ( 11 ) << 0, 1, 0;
			
			//* FUNCTION */
			MatrixXu indices ( 1, N );
			MatrixXf positions ( 3, N );
			MatrixXf colors ( 3, N );
			
			float yRange = 2.0;
			float yMax = 1.0;
			
			for ( size_t i = 0; i < N; i++ ) {
			
				indices.col ( i ) << i;
				
				float x = rand_float ( -1.0, 1.0 );
				float y = rand_float ( -1.0, 1.0 );
				float z = hat ( x, y );
				
				positions.col ( i ) << x, y, z;
				
				colors.col ( i ) = hslToRgb ( 0.6 * ( yMax - z ) / yRange, 1, 0.5 );
				
			}
			
			// bind the shader and upload vertex positions and indices
			
			mShader.bind();
			mShader.uploadIndices ( indices );
			mShader.uploadAttrib ( "vertex_in_position", positions );
			mShader.uploadAttrib ( "vertex_in_color", colors );
			
			//get uniform locations in shaders
			
			uniform_offset = mShader.uniform ( "offset" );
			uniform_angle = mShader.uniform ( "angle" );
			uniform_perspective = mShader.uniform ( "perspective" );
			
			// init camera
			xpos = 0;
			ypos = 0;
			zpos = -4;
			xang = 0;
			yang = 0;
			
			drag_sensitivity = 5.0f;
			keyboard_sensitivity = 0.1f;
			
			// http://code.nabla.net/doc/OpenGL/api/OpenGL/man/glFrustum.html
			//setup perspective
			float fFrustumScale = 3.5f;
			float aspect = ( float ) size() [0] / ( float ) size() [1];
			float fzNear = 0.5f;
			float fzFar = 1000.0f;
			//THE MATRIX
			float theMatrix[16];
			memset ( theMatrix, 0, sizeof ( float ) * 16 );
			//build perspective matrix
			theMatrix[0] = fFrustumScale / aspect;
			theMatrix[5] = fFrustumScale;
			theMatrix[10] = ( fzFar + fzNear ) / ( fzNear - fzFar );
			theMatrix[14] = ( 2 * fzFar * fzNear ) / ( fzNear - fzFar );
			theMatrix[11] = -1.0f;
			
			glUniformMatrix4fv ( uniform_perspective, 1, GL_TRUE, theMatrix ); //perspective matrix
			glUniform3f ( uniform_offset, xpos, ypos, zpos ); //position offset
			glUniform2f ( uniform_angle, xang, yang ); //rotation angles
			
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
			
			//bottom left graph
			graphDyn = add<nanogui::Graph> ( "" );
			graphDyn->values().resize ( HISTORY_SIZE );
			graphDyn->setGraphColor ( nanogui::Color ( 0, 160, 192, 255 ) );
			graphDyn->setBackgroundColor ( nanogui::Color ( 0, 0, 0, 8 ) );
			
			/* console */
			show_console = false;
			
			window_test = new Window ( this, "" );
			window_test->setVisible ( show_console );
			
			// window_test->setLayout ( new GroupLayout ( 5, 5, 0, 0 ) );
			console_test = new nanogui::Console ( window_test );
			console_test->setFontSize ( 12 );
			
			// debug footer message
			footer_message = new nanogui::Console ( this );
			footer_message->setFontSize ( 12 );
			footer_message_string = "";
			
			resizeEvent ( { glfw_window_width, glfw_window_height } );
			
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
			
			if ( key == GLFW_KEY_TAB && action == GLFW_PRESS ) {
			
				show_console = !show_console;
				window_test->setVisible ( show_console );
				
			}
			
			// execute on press or hold, skip on release
			/*	same as process_keyboard(), but this is choppy
				if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
			
				if ( key == GLFW_KEY_W ) ypos = ypos + 0.1 * keyboard_sensitivity;
				if ( key == GLFW_KEY_S ) ypos = ypos - 0.1 * keyboard_sensitivity;
				if ( key == GLFW_KEY_A ) xpos = xpos - 0.1 * keyboard_sensitivity;
				if ( key == GLFW_KEY_D ) xpos = xpos + 0.1 * keyboard_sensitivity;
				if ( key == GLFW_KEY_Q ) zpos = zpos + 0.1 * keyboard_sensitivity;
				if ( key == GLFW_KEY_E ) zpos = zpos - 0.1 * keyboard_sensitivity;
				if ( key == GLFW_KEY_LEFT ) xang = xang - 0.025 * keyboard_sensitivity;
				if ( key == GLFW_KEY_RIGHT ) xang = xang + 0.025 * keyboard_sensitivity;
				if ( key == GLFW_KEY_UP ) yang = yang - 0.025 * keyboard_sensitivity;
				if ( key == GLFW_KEY_DOWN ) yang = yang + 0.025 * keyboard_sensitivity;
			
			}*/
			
			return false;
		}
		
		// smooth keys - TODO: move to nanogui
		void process_keyboard() {
		
			//keyboard management
			if ( glfwGetKey ( glfwWindow(), 'A' ) == GLFW_PRESS ) xpos = xpos - 0.1 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), 'D' ) == GLFW_PRESS ) xpos = xpos + 0.1 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), 'W' ) == GLFW_PRESS ) ypos = ypos + 0.1 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), 'S' ) == GLFW_PRESS ) ypos = ypos - 0.1 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), 'E' ) == GLFW_PRESS ) zpos = zpos - 0.1 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), 'Q' ) == GLFW_PRESS ) zpos = zpos + 0.1 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS ) xang = xang - 0.025 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS ) xang = xang + 0.025 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), GLFW_KEY_UP ) == GLFW_PRESS ) yang = yang - 0.025 * keyboard_sensitivity;
			if ( glfwGetKey ( glfwWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS ) yang = yang + 0.025 * keyboard_sensitivity;
			
		}
		
		/*
		
		virtual bool mouseButtonEvent ( const Eigen::Vector2i &p, int button, bool down, int modifiers ) {
		
					return true;
		
				}
		
		virtual bool mouseMotionEvent ( const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button,
												int modifiers ) {
		
					return true;
		
				}
		*/
		
		virtual bool mouseDragEvent ( const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button,
									  int modifiers ) {
									  
			xang = xang + ( float ) rel[0] / ( ( float ) size() [0] / drag_sensitivity );
			yang = yang + ( float ) rel[1] / ( ( float ) size() [1] / drag_sensitivity );
			
			return true;
			
		}
		
		virtual void draw ( NVGcontext *ctx ) {
		
			process_keyboard();
			graphDyn->values() = Eigen::Map<Eigen::VectorXf> ( FPS.data(), HISTORY_SIZE );
			
			char str[16];
			int last_avg = 10;
			sprintf ( str, "%3.1f FPS\n", graphDyn->values().block ( HISTORY_SIZE - 1 - last_avg, 0, last_avg, 1 ).mean() );
			
			graphDyn->setHeader ( str );
			console_test->setValue ( log_str );
			
			// debug
			std::stringstream ss;
			ss << std::setprecision ( 2 ) << "pos = (" << std::setw ( 5 ) << xpos << ", " <<
			   std::setw ( 5 ) << ypos << ", " << std::setw ( 5 ) << zpos <<
			   ")" << ", ang: (" << xang << ", " << std::setw ( 5 ) << yang << ")" << "\nres = " << size() [0] << "x" << size() [1] <<
			   '\n' << "drag: " << mDragActive << '\n';
			   
			footer_message_string = ss.str();
			footer_message->setValue ( footer_message_string );
			
			/* Draw the user interface */
			Screen::draw ( ctx );
			update_FPS();
			
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
			
			int footer_height = 40;
			int footer_width = 550;
			
			footer_message->setPosition ( {5, 5} );
			footer_message->setSize ( {footer_width, footer_height} );
			
			performLayout();
			
			return true;
			
		}
		
		virtual void drawContents() {
		
		
			mShader.bind();
			
			// glShadeModel ( GL_SMOOTH );
			// glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
			
			// glEnable ( GL_LINE_SMOOTH );
			// glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
			// glEnable ( GL_POINT_SMOOTH );
			// glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
			
			//glEnable ( GL_DEPTH_TEST );
			
			glUniform3f ( uniform_offset, xpos, ypos, zpos );
			glUniform2f ( uniform_angle, xang, yang );
			
			// mShader.drawIndexed ( GL_TRIANGLES, 0, N );
			mShader.drawIndexed ( GL_POINTS, 0, N );
			//glDisable ( GL_DEPTH_TEST );
		}
		
		nanogui::GLShader mShader;
		nanogui::Graph *graphDyn;
		nanogui::Window *window_test;
		nanogui::Console *console_test;
		nanogui::Console *footer_message;
		
		//for manipulating view
		
		float xpos, ypos, zpos, xang, yang;
		GLint uniform_offset, uniform_angle, uniform_perspective;
		
		Eigen::Matrix4f perspective;
		Eigen::Matrix4f view;
		Eigen::Matrix4f modelViewProj;
		
		std::string log_str;
		std::string footer_message_string;
		
		bool show_console;
		float drag_sensitivity;
		float keyboard_sensitivity;
		
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
