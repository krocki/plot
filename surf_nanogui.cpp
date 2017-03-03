/*
* @Author: Kamil Rocki
* @Date:   2017-02-28 11:25:34
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-02 18:46:01
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
			nanogui::Screen ( Eigen::Vector2i ( 800, 500 ), "Surface Plot", true, fullscreen, 8, 8, 24, 8, aliasing_samples, 3,
							  3 ) {
							  
			load_shaders();
			init();
			
		}
		
		void load_shaders() {
		
			size_t sz_vertShader, sz_fragShader;
			size_t sz_geomShader;
			
			char *vertShader = file_read ( "passthrough.v.glsl", &sz_vertShader );
			char *fragShader = file_read ( "passthrough.f.glsl", &sz_fragShader );
			// char *geomShader = file_read ( "passthrough.g.glsl", &sz_geomShader );
			
			//bool initFromFiles(const std::string &name, const std::string &vertex_fname, const std::string &fragment_fname, const std::string &geometry_fname = "")
			
			mShader.init ( "passthrough", string ( vertShader, sz_vertShader ), string ( fragShader, sz_fragShader ) );
			
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
			
			// bind the shader and upload vertex positions and indices
			
			mShader.bind();
			mShader.uploadIndices ( indices );
			mShader.uploadAttrib ( "vertex_in_position", positions );
			mShader.uploadAttrib ( "vertex_in_color", colors );
			
			translation.setIdentity();
			rotation.setIdentity();
			scale.setIdentity();
			theta = Eigen::Vector3f::Zero();
			rotate_step = 0.05f;
			
			// instead of gluPerspective(double fov, double aspect, double near, double far)
			
			// http://stackoverflow.com/questions/13768423/setting-up-projection-model-and-view-transformations-for-vertex-shader-in-eige
			
			double aspect = ( double ) size() [0] / ( double ) size() [1];
			double fov = 0.6f;
			double near = 0.2f;
			double far = 100.0f;
			
			float range = far - near;
			float invtan = 1. / tan ( fov * 0.5f );
			
			perspective ( 0, 0 ) = invtan / aspect;
			perspective ( 1, 1 ) = invtan;
			perspective ( 2, 2 ) = - ( near + far ) / range;
			perspective ( 3, 2 ) = -1;
			perspective ( 2, 3 ) = -2 * near * far / range;
			perspective ( 3, 3 ) = 0;
			
			// view
			view = Eigen::Matrix4f::Identity();
			
			Eigen::Matrix3f R;
			Eigen::Vector3f position;
			position << 0, 0, -5; // eye position
			Eigen::Vector3f target;
			target << 0, 0, 0; // looking at
			Eigen::Vector3f up;
			up << 0, 1, 0; // normal vector
			
			R.col ( 2 ) = ( position - target ).normalized();
			R.col ( 0 ) = up.cross ( R.col ( 2 ) ).normalized();
			R.col ( 1 ) = R.col ( 2 ).cross ( R.col ( 0 ) );
			
			view.topLeftCorner<3, 3>() = R.transpose();
			view.topRightCorner<3, 1>() = -R.transpose() * position;
			view ( 3, 3 ) = 1.0f;
			
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
			
			if ( key == GLFW_KEY_W ) theta ( 0 ) += rotate_step;
			
			if ( key == GLFW_KEY_S ) theta ( 0 ) -= rotate_step;
			
			if ( key == GLFW_KEY_A ) theta ( 1 ) += rotate_step;
			
			if ( key == GLFW_KEY_D ) theta ( 1 ) -= rotate_step;
			
			if ( key == GLFW_KEY_Q ) theta ( 2 ) += rotate_step;
			
			if ( key == GLFW_KEY_E ) theta ( 2 ) -= rotate_step;
			
			return false;
		}
		
		virtual void draw ( NVGcontext *ctx ) {
		
			// glShadeModel ( GL_SMOOTH );
			// glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
			
			// glEnable ( GL_LINE_SMOOTH );
			// glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
			// glEnable ( GL_POINT_SMOOTH );
			// glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
			
			// glEnable ( GL_BLEND );
			// glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			// glEnable ( GL_DEPTH_TEST ); // enable depth-testing
			// glDepthFunc ( GL_LESS ); // depth-testing interprets a smaller value as "closer"
			// glClearColor ( 0.5f, 0.5f, 0.5f, 0.5f );
			// //glEnable(GL_CULL_FACE);
			// glCullFace ( GL_BACK );
			// glFrontFace ( GL_CCW );
			
			graphDyn->values() = Eigen::Map<Eigen::VectorXf> ( FPS.data(), HISTORY_SIZE );
			
			char str[16];
			int last_avg = 10;
			sprintf ( str, "%3.1f FPS\n", graphDyn->values().block ( HISTORY_SIZE - 1 - last_avg, 0, last_avg, 1 ).mean() );
			
			graphDyn->setHeader ( str );
			console_test->setValue ( log_str );
			
			// debug
			std::stringstream ss;
			ss << std::setprecision ( 2 ) << "theta = (" << std::setw ( 5 ) << theta[0] << ", " <<
			   std::setw ( 5 ) << theta[1] << ", " << std::setw ( 5 ) << theta[2] <<
			   ")" << '\n' << "res = " << size() [0] << "x" << size() [1] << '\n';
			   
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
			
			int footer_height = 30;
			int footer_width = 550;
			
			footer_message->setPosition ( {5, 5} );
			footer_message->setSize ( {footer_width, footer_height} );
			
			performLayout();
			
			return true;
			
		}
		
		virtual void drawContents() {
		
			mShader.bind();
			
			glEnable ( GL_DEPTH_TEST );
			/* Draw 12 triangles starting at index 0 */
			
			Eigen::Vector3f c = Eigen::Vector3f::Zero(); // center of rotation
			Eigen::AngleAxisf x = Eigen::AngleAxisf ( theta ( 0 ), Eigen::Vector3f::UnitX() );
			Eigen::AngleAxisf y = Eigen::AngleAxisf ( theta ( 1 ), Eigen::Vector3f::UnitY() );
			Eigen::AngleAxisf z = Eigen::AngleAxisf ( theta ( 2 ), Eigen::Vector3f::UnitZ() );
			
			Eigen::Quaternion<float> q = z * y * x;
			
			Eigen::Affine3f model = Eigen::Translation3f ( c ) * q * Eigen::Translation3f ( -c );
			
			modelViewProj = perspective * view * model.matrix();
			glLoadMatrixf ( modelViewProj.data() );
			
			mShader.setUniform ( "modelViewProj", modelViewProj );
			mShader.drawIndexed ( GL_TRIANGLES, 0, 12 );
			glDisable ( GL_DEPTH_TEST );
			
		}
		
		nanogui::GLShader mShader;
		nanogui::Graph *graphDyn;
		nanogui::Window *window_test;
		nanogui::Console *console_test;
		nanogui::Console *footer_message;
		
		//for manipulating view
		
		float rotate_step;
		Eigen::Vector3f theta;
		Eigen::Matrix4f translation;
		Eigen::Matrix4f rotation;
		Eigen::Matrix4f scale;
		
		Eigen::Matrix4f perspective;
		Eigen::Matrix4f view;
		Eigen::Matrix4f modelViewProj;
		
		std::string log_str;
		std::string footer_message_string;
		
		bool show_console;
		
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
