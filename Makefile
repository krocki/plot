GLFW_PATH=/usr/local/Cellar/glfw/3.2.1
GLM_PATH=/usr/local/Cellar/glm/0.9.8.3
GLEW_PATH=/usr/local/Cellar/glew/2.0.0/

#need the flags for OpenGL 4.1 !
FLAGS = -DAPPLE -Wfatal-errors -Wall -pedantic -mmacosx-version-min=10.9 -arch x86_64 -fmessage-length=0 -UGLFW_CDECL

all:
	# g++ -o hello main.cc -lglfw -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -framework OpenGL
	# g++ -o example example.cc -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o keynames keynames.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o viewports viewports.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o modes modes.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o bezier bezier.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o vulcan vulcan.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	# g++ -o multiple_plots multiple_plots.cc -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	#gcc -o graph graph.c -lglfw -lglew $(FLAGS) -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit
	g++ -o multiple_windows multiple_windows.cc -stdlib=libc++ -lglfw -lglew $(FLAGS) -I. -I$(GLM_PATH)/include/ -I$(GLEW_PATH)/include/ -I$(GLFW_PATH)/include/ -L$(GLFW_PATH)/lib/ -L$(GLEW_PATH)/lib/ -framework Cocoa -framework OpenGL -framework IOKit