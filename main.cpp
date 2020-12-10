#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Renderer.h"
#include "src/model.h"
#include "src/Trees.h"
#include "src/Terrain.h"
#include "src/stb_image.h"
#include "src/Movobj.h"
#include "src/Aerialobj.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/Shaderr.h"

#include <set>
#include <iostream>
#include <iomanip>

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

//Callbacks********************************************************
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//*****************************************************************

Camera camera(glm::vec3(5.0f, 0.5f, 0.0f));
const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 1200;

//CAMERA CLASS HERE
float lastX = SCREEN_WIDTH / 2.f;
float lastY = SCREEN_HEIGHT / 2.f;
bool firstMouse = true;

//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{
	GLFWwindow* window;

	// Initialization of GLFW
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed." << '\n';
		std::cin.get();

		return EXIT_FAILURE;
	}

	// Setting window hints
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1200, 1200, "Assigment two - Pacman 3D", NULL, NULL);
	if (!window)
	{
		std::cerr << "GLFW failed on window creation." << '\n';
		std::cin.get();

		glfwTerminate();

		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);									   //Make the window's context current
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Setting callbacks
	glfwSetCursorPosCallback(window,       mouse_callback);				  
	glfwSetScrollCallback(window,          scroll_callback);					  
	glfwSetKeyCallback(window,             key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);       //Tell GLFW to capture our mouse

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glEnable(GL_DEPTH_TEST); //IMPORTANT FOR 3D

	Shader					shader("shaders/terrainVS.glsl", "shaders/terrainFS.glsl");
	Renderer				renderer;
	Terrain					terrain(&shader, &renderer, 1080, 1080, 1.0f);

	Shader					treeShader("shaders/treeVS.glsl", "shaders/treeFS.glsl");
	Model					tree("res/objects/PineTree2/10447_Pine_Tree_v1_L3b.obj");
	Trees					trees(&tree, &terrain);

	Model					airplane("res/objects/Plane/Plane.obj");
	Shader					planeShader("shaders/gObjectsVS.glsl", "shaders/gObjectsFS.glsl");
	Aerialobj				plane(&airplane);

	Model					deer("res/objects/Deer1/12961_White-Tailed_Deer_v1_l2.obj");
	std::vector<Shader*>    deerShaders;
	std::vector<Movobj*>	deers;

	for (int i = 0; i < 18; i++) {
		deerShaders.push_back(new Shader("shaders/gObjectsVS.glsl", "shaders/gObjectsFS.glsl"));
		deers.push_back(new Movobj(&terrain, &deer, i, &camera));
		deers[i]->setSpawn();
	}

	glfwSetTime(0);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//DT
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.065f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 2000.0f);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();

		terrain.m_Shader->use();
		terrain.Light(deltaTime, &camera, currentFrame);
		terrain.draw(projection, view, deltaTime);

		trees.Draw(&treeShader, projection, view); //instanced trees

		for (int i = 0; i < 18; i++) {
			deers[i]->draw(deerShaders[i], projection, view, deltaTime, currentFrame);
		}

		plane.draw(&planeShader, projection, view, deltaTime, currentFrame);

		processInput(window);
		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	for (auto obj : deerShaders) //Deleting pointers
		delete obj;

	for (auto obj : deers)
		delete obj;

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		/*camera->togglePerspective();
		std::cout << "Perspective: " << camera->perspective << std::endl;*/

		camera.togglePerspective();
		std::cout << "Perspective: " << camera.perspective << std::endl;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//camera->ProcessMouseMovement(xoffset, yoffset);
	camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? " GL ERROR " : "") <<
		"type = 0x" << type <<
		", severity = 0x" << severity <<
		", message =" << message << "\n";
}