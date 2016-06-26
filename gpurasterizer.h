#ifndef GPURASTERIZER_H
#define GPURASTERIZER_H

#include <geotypes.h> 
#include <vector> 
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h> 

#include <triangle.h>

// DEBUG
#include <iostream>

class GpuRasterizer
{
private:
	GLFWwindow* window;

	static void error_callback(int error, const char* description)
	{
		 fputs(description, stderr);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		 if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			  glfwSetWindowShouldClose(window, GL_TRUE);
	}

public:
	GpuRasterizer()
	{
		glfwSetErrorCallback(error_callback);
		if (!glfwInit())
			 exit(EXIT_FAILURE);

		window = glfwCreateWindow(150, 150, "Voxelizer", NULL, NULL);
		if (!window)
		{
			 glfwTerminate();
			 exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		glfwSetKeyCallback(window, key_callback);		
	}

void Rasterize(std::vector<Segment2> segments)
{
	struct triangulateio in, mid, out, vorout;

	in.numberofsegments = segments.size();
	in.numberofholes = 0;
	in.numberofregions = 0;

	//unsigned char buffer[100*100*3];
       int width, height;

       glfwGetFramebufferSize(window, &width, &height);
       glViewport(0, 0, width, height);
       glClear(GL_COLOR_BUFFER_BIT);

       //glMatrixMode(GL_PROJECTION);
       //glLoadIdentity();
       //glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
       //glMatrixMode(GL_MODELVIEW);
       //glLoadIdentity();
       //glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

	glColor3f(0.0,0.0,1.0);
       glBegin(GL_LINES);
        	 	for(std::vector<Segment2>::iterator i = segments.begin(); i != segments.end(); i++)	
        	 	{
        	 		glVertex2f((*i)[0].x/32, (*i)[0].y/32);
        	 		glVertex2f((*i)[1].x/32, (*i)[1].y/32);
        	 	}
        glEnd();

       //glBegin(GL_TRIANGLES);
       //glVertex3f(-0.6f, -0.4f, 0.f);
       //glVertex3f(0.6f, -0.4f, 0.f);
       //glVertex3f(0.f, 0.6f, 0.f);
       //glEnd();

		 //glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);	

		 //std::cout << "Hello:" << std::endl;
		 //for (int i = 0; i < width*height*3; i+=3)
		 //  std::cout << std::hex << static_cast<int>(buffer[i]) << ',' << static_cast<int>(buffer[i+1]) << ',' << static_cast<int>(buffer[i+2]) << std::endl;
		
       glfwSwapBuffers(window);

       glfwPollEvents();

		//	//for(std::vector<Segment2>::iterator i = segments.begin(); i != segments.end(); i++)	
		//	//{
		//	//	glVertex2f((*i)[0].x/32, (*i)[0].y/32);
		//	//	glVertex2f((*i)[1].x/32, (*i)[1].y/32);
		//	//}
		//glEnd();

		//glReadPixels(0, 0, 100, 100, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);	

		//for (int i = 0; i < 10*3; i+=3)
		//	std::cout << buffer[i] << ',' << buffer[i+1] << ',' << + buffer[i+2] << std::endl;
	}

	~GpuRasterizer()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
};

#endif
