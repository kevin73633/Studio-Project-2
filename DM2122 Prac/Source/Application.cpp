//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Application.h"

#include "SceneText.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
float screenoffsetx;
float screenoffsety;
//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	//update opengl new window size
	glViewport(0, 0, w, h);
}
void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
	screenoffsetx = xpos;
	screenoffsety = ypos;
}
bool Application::IsKeyPressed(unsigned short key)
{

	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);


	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);
	screenoffsetx = 146;
	screenoffsety = 213;
	glfwSetWindowPos(m_window, screenoffsetx, screenoffsety);
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetWindowPosCallback(m_window, window_pos_callback);
}

double Application::getmouseXpos()//get mpouse x coord
{
	double xpos, ypos;
	//getting cursor position
	glfwGetCursorPos(m_window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return (xpos - width / 2);
}
double Application::getmouseYpos()//get mouse y coord
{
	double xpos, ypos;
	//getting cursor position
	glfwGetCursorPos(m_window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return (ypos - height / 2);
}
void Application::mouseupdate()//reset mouse position
{
	ShowCursor(false);
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	SetCursorPos(screenoffsetx + width / 2, screenoffsety + height / 2);
}
void Application::Run()
{
	//Main Loop

	Scene* scene = new SceneText();
	scene->Init();
	glfwWindowHint(GLFW_CENTER_CURSOR, true);

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}