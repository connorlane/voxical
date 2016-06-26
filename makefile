LIBS := $(shell pkg-config --libs --static glfw3)

volexizer: main.cpp geotypes.h stlparser.h gpurasterizer.h
	g++ main.cpp -I. -o voxelizer.x -std=c++0x -Wall $(LIBS)

opengl_test: opengl_test.cpp gpurasterizer.h
	g++ opengl_test.cpp -I. -o opengl_test -std=c++0x -Wall $(LIBS)

rasterize_test: rasterize_test.cpp gpurasterizer.h
	g++ rasterize_test.cpp -I. -o rasterize_test -std=c++0x -Wall $(LIBS)

clean: 
	rm -f voxelizer.x
