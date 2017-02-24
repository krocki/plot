/*
* @Author: Kamil Rocki
* @Date:   2017-02-23 14:57:38
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-02-23 15:16:31
*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class BasicApp : public App {
	public:
		void draw() override;
};

// https://libcinder.org/docs/guides/opengl/part1.html

void BasicApp::draw() {
	gl::clear();
	vec2 center = getWindowCenter();
	float r = 100;
	
	gl::color ( Color ( 1, 0, 0 ) ); // red
	gl::drawSolidCircle ( center + vec2 ( -r, r ), r );
	gl::color ( Color ( 0, 1, 0 ) ); // green
	gl::drawSolidCircle ( center + vec2 ( r, r ), r );
	gl::color ( Color ( 0, 0, 1 ) ); // blue
	gl::drawSolidCircle ( center + vec2 ( 0, -0.73 * r ), r );
}

CINDER_APP ( BasicApp, RendererGl )