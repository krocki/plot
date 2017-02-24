GLFW_PATH=/usr/local/Cellar/glfw/3.2.1
GLM_PATH=/usr/local/Cellar/glm/0.9.8.3
GLEW_PATH=/usr/local/Cellar/glew/2.0.0
FREETYPE_PATH=/usr/local/Cellar/freetype/2.7.1
FREEGLUT_PATH=/usr/local/Cellar/freeglut/3.0.0
EIGEN_PATH=/usr/local/Cellar/eigen/3.3.1
SDL2_PATH=/usr/local/include/SDL2
CINDER_PATH=/Applications/cinder_0.9.0_mac
NANOVG_PATH=./nanovg/

#need the flags for OpenGL 4.1 !
FLAGS = -DAPPLE -Wfatal-errors -Wall -pedantic -mmacosx-version-min=10.9 -arch x86_64 -fmessage-length=0 -UGLFW_CDECL
CFLAGS = -Ofast

all:
	# g++ -o hello main.cc -lglfw -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -framework OpenGL
	# g++ -o example example.cc -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o keynames keynames.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o viewports viewports.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o modes modes.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o bezier bezier.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o vulcan vulcan.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o multiple_plots multiple_plots.cc -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# gcc -o graph graph.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o multiple_windows multiple_windows.cc -stdlib=libc++ -lglfw -lglew $(FLAGS) -I. -I$(GLM_PATH)/include/ -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o text text.cc -stdlib=libc++ -lglfw -lfreetype -lglew -lglut $(FLAGS) -I. -I$(GLM_PATH)/include/ -I$(FREEGLUT_PATH)/include/ -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -I$(FREETYPE_PATH)/include/freetype2/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -L$(FREEGLUT_PATH)/lib/ -L$(FREETYPE_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o graph-sdl graph-sdl.cc -D_THREAD_SAFE -lGLEW -lSDL2 -I$(SDL2_PATH) -L/usr/local/lib -I$(GLEW_PATH)/include/ -Ofast -Wall -I$(GLM_PATH)/include/ -L$(GLEW_PATH)/lib/ -framework OpenGL
	# g++ -o nanogui -I/usr/local/include/ -I$(EIGEN_PATH)/include/eigen3/ nanogui.cc
	# g++ -std=c++11 -stdlib=libc++ -o cinder cinder.cc -I$(CINDER_PATH)/include -L$(CINDER_PATH)/lib -Wfatal-errors -lcinder -framework Cocoa -framework OpenGL -framework IOKit -framework Accelerate -framework CoreVideo -framework AVFoundation -framework CoreMedia -framework IOSurface -framework AudioToolbox -framework CoreAudio

	# nanovg
	# gcc -c ./nanovg/nanovg.c $(CFLAGS) -o ./nanovg/nanovg.o
	# gcc -c perf.c $(CFLAGS) -o perf.o -I. -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ 
	# gcc -c demo.c $(CFLAGS) -o demo.o -I. -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ 

	# gcc example_gl2.c -o nanovg_gl2 perf.o demo.o ./nanovg/nanovg.o $(CFLAGS) -lglfw -lglew -I. -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# gcc example_gl3.c -o nanovg_gl3 perf.o demo.o ./nanovg/nanovg.o $(CFLAGS) -lglfw -lglew -I. -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# gcc example_fbo.c -o nanovg_fbo perf.o demo.o ./nanovg/nanovg.o $(CFLAGS) -lglfw -lglew -I. -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit

	#nanogui
	# g++ -std=c++11 -stdlib=libc++ -L/usr/local/lib/ -I$(NANOVG_PATH) -I/usr/local/include -I$(EIGEN_PATH)/include/eigen3/ -o nanogui_test nanogui_test.cc -lnanogui -framework Cocoa -framework OpenGL -framework IOKit

	g++ -std=c++11 -stdlib=libc++ -L/usr/local/lib/ -I$(NANOVG_PATH) -I/usr/local/include -I$(EIGEN_PATH)/include/eigen3/ -o nanogui_example1 example1.cc -lnanogui -framework Cocoa -framework OpenGL -framework IOKit
	g++ -std=c++11 -stdlib=libc++ -L/usr/local/lib/ -I$(NANOVG_PATH) -I/usr/local/include -I$(EIGEN_PATH)/include/eigen3/ -o nanogui_example4 example4.cc -lnanogui -framework Cocoa -framework OpenGL -framework IOKit