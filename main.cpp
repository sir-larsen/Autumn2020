#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/ScenarioLoader.h"
#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Renderer.h"
#include "src/model.h"
#include "src/Trees.h"

#include "src/Terrain.h"
#include "src/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/Shaderr.h"

#include <set>
#include <iostream>
#include <iomanip>

/*struct Vertex
{
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};
std::vector<Vertex> vertices;
*/
/*std::vector <GLuint> gCubes;
std::vector <unsigned int> mapIndices;
std::vector <glm::vec3> mapPositions;
std::vector <glm::vec3> mapNormals;*/

/*glm::vec3 cameraPos = glm::vec3(0.0f, 0.35f, 3.0f); //Position of camera
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //Vector pointing to camera positive y-axis*/

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
//*****************************************************************

//void setUniforms(Maze3D* maze);

//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

Camera* camera;
const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 1200;

//CAMERA CLASS HERE
float lastX = SCREEN_WIDTH / 2.f;
float lastY = SCREEN_HEIGHT / 2.f;
bool firstMouse = true;

/******************************/
bool constrainMovement = true;
bool gameover = false;
/******************************/



//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//GLuint createMaze();
GLuint createCube(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1);
GLuint CompileShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
//void Camera(const float time, const GLuint shaderprogram, float bevX, float bevY, float bevZ);
//void Camera2(const float time, const GLuint shaderprogram, float bevX, float bevY, float bevZ);
//void Light(const float time, const GLuint shaderprogram);
//void Input(GLFWwindow* window, double dt);
void CleanVAO(GLuint& vao);

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
	glfwSetCursorPosCallback(window, mouse_callback);				   //
	glfwSetScrollCallback(window, scroll_callback);					   //
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);       //Tell GLFW to capture our mouse

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glEnable(GL_DEPTH_TEST); //IMPORTANT FOR 3D

	//Shadow stuff
	const GLuint SHADOW_WIDTH = 1024;
	const GLuint SHADOW_HEIGHT = 1024;


	ScenarioLoader  scenario("levels/level0");
	//Shader			shader("shaders/camera2.glsl");
	Shader			shader("shaders/mazeVS.glsl", "shaders/mazeFS.glsl");
	Renderer		renderer;
	Terrain			terrain(&scenario, &shader, &renderer, 1080, 1080, 1.0f);
	camera =        new Camera(glm::vec3(5.0f, 0.5f, 0.0f));

	std::cout << "Treecount: " << terrain.getTreeCount() << std::endl;

	Model ghost("res/ghost/Ghost.obj");
	Shader ghostShader/*= new Shader*/("shaders/ghostVS.glsl", "shaders/ghostFS.glsl");

	Shader treeShader("shaders/treeVS.glsl", "shaders/treeFS.glsl");
	Model tree("res/objects/PineTree2/10447_Pine_Tree_v1_L3b.obj");
	Trees trees(&tree, &terrain);
	





	glfwSetTime(0);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//DT
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//glClearColor(0.5f, 0.2f, 0.1f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

		/*if (cameraPos.y > 0.353f)
			cameraPos.y = 0.35f;
		if (cameraPos.y < 0.347f)
			cameraPos.y = 0.35f;*/
		

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 2000.0f);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();

		terrain.m_Shader->use();
		terrain.Light(deltaTime, camera, currentFrame);
		terrain.draw(projection, view, deltaTime); //Draw call

		trees.Draw(&treeShader, projection, view);

		/*Drawing of ghost object*/
		ghostShader.use();
		ghostShader.setMat4("u_ProjectionMat", projection);
		ghostShader.setMat4("u_ViewMat", view);
		glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.5f, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(.3f));
		//glm::mat4 rotation = glm::mat4(1);
		glm::mat4 transformation = translation * /*rotation */ scale;
		ghostShader.setMat4("u_TransformationMat", transformation);
		Shader* gsPoint = &ghostShader;
		//ghost.Draw(&(Shader)ghostShader);
		ghost.Draw(*gsPoint);

		ghostShader.setMat4("u_ProjectionMat", projection);
		ghostShader.setMat4("u_ViewMat", view);
		translation = glm::translate(glm::mat4(1), glm::vec3(2.0f, 0.5f, 2.0f));
		scale = glm::scale(glm::mat4(1), glm::vec3(0.01f));
		transformation = translation * /*rotation */ scale;
		ghostShader.setMat4("u_TransformationMat", transformation);
		Shader* treePoint = &ghostShader;
		tree.Draw(*treePoint);

		/*************************/
		
		
		processInput(window);

		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}

		/* Poll for and process events */
		glfwPollEvents();


	}

	glfwTerminate();
	return 0;
}


// -----------------------------------------------------------------------------
// Clean VAO
// -----------------------------------------------------------------------------
void CleanVAO(GLuint& vao)
{
	GLint nAttr = 0;
	std::set<GLuint> vbos;

	GLint eboId;
	glGetVertexArrayiv(vao, GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboId);
	glDeleteBuffers(1, (GLuint*)&eboId);

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
	glBindVertexArray(vao);

	for (int iAttr = 0; iAttr < nAttr; ++iAttr)
	{
		GLint vboId = 0;
		glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
		if (vboId > 0)
		{
			vbos.insert(vboId);
		}

		glDisableVertexAttribArray(iAttr);
	}

	for (auto vbo : vbos)
	{
		glDeleteBuffers(1, &vbo);
	}

	glDeleteVertexArrays(1, &vao);
}

GLuint CompileShader(const std::string& vertexShaderSrc,
	const std::string& fragmentShaderSrc)
{

	auto vertexSrc = vertexShaderSrc.c_str();
	auto fragmentSrc = fragmentShaderSrc.c_str();

	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
	glCompileShader(vertexShader);
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		std::cout << errorLog.data() << std::endl;
		std::cin.get();
	}
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentShader);

	//I spent like 3 hours trying to find shader errors. Here you go. Now this will print any error you get trying to compile a shader.
	//Don't suffer like I did.

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		std::cout << errorLog.data() << std::endl;
		std::cin.get();
	}

	auto shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


GLuint createCube(GLfloat x0, GLfloat x1, GLfloat z0, GLfloat z1)
{
	GLfloat cube[] =
	{
		//FRONT FACE
		x0, 0.0f, z0, //--> Indice nr 0 (0,0,0) in cube origo model
		x0, 0.0f, z1, //--> Indice nr 1 (0,0,1) in CO model
		x0, 3.0f, z1, //--> Indice nr 2 (0,1,1) in CO model
		x0, 3.0f, z0, //--> Indice nr 3 (0,1,0) in CO model

		//BACK FACE
		x1, 0.0f, z0, //--> Indice nr 4 (1,0,0)
		x1, 0.0f, z1, //--> Indice nr 5 (1,0,1)
		x1, 3.0f, z1, //--> Indice nr 6 (1,1,1)
		x1, 3.0f, z0  //--> Indice nr 7 (1,1,0)
	};
	GLuint cubeIndices[36] = //Elementbuffer
	{
		//Front
		0,1,2 , 2,3,0,
		//Back
		4,5,6 , 6,7,4,
		//Top
		3,2,6 , 6,7,3,
		//Bottom
		0,1,5 , 5,4,0,
		//Left
		4,0,3 , 3,7,4,
		//Right
		1,5,6 , 6,2,1
	};

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

	return vao;

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);

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

	camera->ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
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