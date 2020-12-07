#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/ScenarioLoader.h"
#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Renderer.h"
#include "src/model.h"

#include "src/Maze3D.h"
#include "src/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/Shaderr.h"

#include <set>
#include <iostream>
#include <iomanip>
#include "src/Maze3D.h"

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
	Maze3D			maze(&scenario, &shader, &renderer);
	camera =        new Camera(/*glm::vec3(0.0f, 0.0f, 3.0f)*/maze.findSpawn());

	Model ghost("res/ghost/Ghost.obj");
	Shader ghostShader/*= new Shader*/("shaders/ghostVS.glsl", "shaders/ghostFS.glsl");





	/*int indexOff = 0;
	glm::vec3 offset;

	int height, width;
	height = 36, width = 28;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			offset = glm::vec3(i, 0.0f, j);

			//Adding 24 vertices and 36 indices
			if (i < map.size() && j < map[i].size() && map[i][j] == 1) {
				Vertex vertex3dNormTex;

				//Adding bottom face

				vertex3dNormTex.location  = offset + glm::vec3(0.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals   = glm::vec3(0.0f, -1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding top face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding left face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding right face
				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding north face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 0.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

				//Adding south face
				vertex3dNormTex.location = offset + glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 0.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(1.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				vertex3dNormTex.location = offset + glm::vec3(0.0f, 1.0f, 1.0f);
				vertex3dNormTex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
				vertex3dNormTex.texCoords = glm::vec2(0.0f, 0.0f);
				vertices.push_back(vertex3dNormTex);

				mapIndices.push_back(indexOff + 0);
				mapIndices.push_back(indexOff + 1);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 2);
				mapIndices.push_back(indexOff + 3);
				mapIndices.push_back(indexOff + 0);
				indexOff += 4;

			}


		}
	}*/


	/*for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 28; j++) {
			/*if (map[i][j] == 1) {
				GLuint cubeVao = createCube((GLfloat)j, (GLfloat)j + 1, (GLfloat)i, (GLfloat)i + 1);
				gCubes.push_back(cubeVao);
			}
			if (map[i][j] == 2) {
				cameraPos = glm::vec3((GLfloat)i+0.5f, 0.5f, (GLfloat)j+0.5f);
				std::cout << "i: " << i << " j: " << j << std::endl;
			}
		}
	}*/

	//GLuint mazeVAO = createMaze();

	
	//GLuint vao = createCube(0.f, 1.f, 0.f, 1.f);
	//GLuint shaderProgram = CompileShader(VertexShaderSrc, /*directionalLightFragmentShaderSrc*/pointLightFragmentShaderSrc);

	glfwSetTime(0);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//DT
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glUseProgram(shaderProgram);
		
		GLuint vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
		glUniform4f(vertexColorLocation, 0.1f, 0.9f, 0.1f, 1.0f);
		glBindVertexArray(mazeVAO);
		glDrawElements(GL_TRIANGLES, mapIndices.size(), GL_UNSIGNED_INT, (const void*)0);*/

				

		/*if (cameraPos.y > 0.353f)
			cameraPos.y = 0.35f;
		if (cameraPos.y < 0.347f)
			cameraPos.y = 0.35f;*/
		


		/*maze.m_Shader->setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
		maze.m_Shader->setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		maze.m_Shader->setVec3("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
		maze.m_Shader->setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);*/


		/*// point light 1
		maze.m_Shader->setUniform3f("pointLight.position", camera.Position);
		maze.m_Shader->setUniform3f("pointLight.ambient", 0.05f, 0.05f, 0.05f);
		maze.m_Shader->setUniform3f("pointLight.diffuse", 0.1f, 0.1f, 0.1f);
		maze.m_Shader->setUniform3f("pointLight.specular", 0.1f, 0.1f, 0.1f);
		maze.m_Shader->setUniform1f("pointLight.constant", 1.f);
		maze.m_Shader->setUniform1f("pointLight.linear", 0.09);
		maze.m_Shader->setUniform1f("pointLight.quadratic", 0.032);*/


		//Spotlight
		//maze.m_Shader->setUniform3f("spotLight.position", camera.Position);
		//maze.m_Shader->setUniform3f("spotLight.direction", camera.Front);

		/*glUniform3f(glGetUniformLocation(maze.m_Shader->ID, "spotLight.position"), camera->Position.x, camera->Position.y, camera->Position.z);
		glUniform3f(glGetUniformLocation(maze.m_Shader->ID, "spotLight.direction"), camera->Front.x, camera->Front.y, camera->Front.z);

		maze.m_Shader->setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
		maze.m_Shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		maze.m_Shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		maze.m_Shader->setFloat("spotLight.constant", 1.0f);
		maze.m_Shader->setFloat("spotLight.linear", 0.09);
		maze.m_Shader->setFloat("spotLight.quadratic", 0.032);
		maze.m_Shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(26.5f)));
		maze.m_Shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(30.0f)));*/


		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();

		maze.m_Shader->use();
		maze.Light(deltaTime, camera);
		maze.draw(projection, view, deltaTime); //Draw call

		/*Drawing of ghost object*/
		ghostShader.use();
		ghostShader.setMat4("u_ProjectionMat", projection);
		ghostShader.setMat4("u_ViewMat", view);
		glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(maze.findSpawn()));
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(.3f));
		//glm::mat4 rotation = glm::mat4(1);
		glm::mat4 transformation = translation * /*rotation */ scale;
		ghostShader.setMat4("u_TransformationMat", transformation);
		Shader* gsPoint = &ghostShader;
		//ghost.Draw(&(Shader)ghostShader);
		ghost.Draw(*gsPoint);

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

	//glUseProgram(0);
	//glDeleteProgram(shaderProgram);

	//CleanVAO(squareVAO); //Rydder opp elns??
	/*for (auto& val : gCubes) {
		CleanVAO(val);
	}*/

	glfwTerminate();
	return 0;
}

/*void setUniforms(Maze3D* maze)
{
	// point light 1
	maze.m_Shader->setUniform3f("pointLight.position", glm::vec3(14, 3, 18));
	maze.m_Shader->setUniform3f("pointLight.ambient", 0.05f, 0.05f, 0.05f);
	maze.m_Shader->setUniform3f("pointLight.diffuse", 0.1f, 0.1f, 0.1f);
	maze.m_Shader->setUniform3f("pointLight.specular", 0.1f, 0.1f, 0.1f);
	maze.m_Shader->setUniform1f("pointLight.constant", 1.f);
	maze.m_Shader->setUniform1f("pointLight.linear", 0.09);
	maze.m_Shader->setUniform1f("pointLight.quadratic", 0.032);



	maze.m_Shader->setUniform3f("spotLight.position", camera.Position);
	maze.m_Shader->setUniform3f("spotLight.direction", camera.Front);
	maze.m_Shader->setUniform3f("spotLight.ambient", 0.1f, 0.1f, 0.1f);
	maze.m_Shader->setUniform3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	maze.m_Shader->setUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
	maze.m_Shader->setUniform1f("spotLight.constant", 1.0f);
	maze.m_Shader->setUniform1f("spotLight.linear", 0.09);
	maze.m_Shader->setUniform1f("spotLight.quadratic", 0.032);
	maze.m_Shader->setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	maze.m_Shader->setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}*/


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

/*GLuint createMaze()
{
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapIndices.size() * sizeof(unsigned int), &mapIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Enabling the positions in the shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);

	glEnableVertexAttribArray(1); //Enabling the normals in the shader file
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

	//glEnableVertexAttribArray(2); //Enabling textures in the shader
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	return vao;
}*/

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

/*void Camera2(const float time, const GLuint shaderprogram, float bevX, float bevY, float bevZ)
{
	
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)1400 / 1000, 0.1f, 100.f);
	//Creating our LookAt matrix
	glm::mat4 view;					//for "strafing" effect
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//Get unforms to place our matrices into
	GLuint projmat = glGetUniformLocation(shaderprogram, "u_ProjectionMat");
	GLuint viewmat = glGetUniformLocation(shaderprogram, "u_ViewMat");

	//Send data from matrices to uniform
	glUniformMatrix4fv(projmat, 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(viewmat, 1, false, glm::value_ptr(view));
}*/

/*void Camera(const float time, const GLuint shaderprogram, float bevX, float bevY, float bevZ)
{
	//Matrix which helps project our 3D objects onto a 2D image. Not as relevant in 2D projects
	//The numbers here represent the aspect ratio. Since our window is a square, aspect ratio here is 1:1, but this can be changed.
	glm::mat4 projection = glm::perspective(90.f, 1.f, 0.1f, 60.f);

	//Matrix which defines where in the scene our camera is
	//                           Position of camera     Direction camera is looking     Vector pointing upwards
	glm::mat4 view = glm::lookAt(glm::vec3(2.f + bevX, 5.f + bevY, 3.f + bevZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//Get unforms to place our matrices into
	GLuint projmat = glGetUniformLocation(shaderprogram, "u_ProjectionMat");
	GLuint viewmat = glGetUniformLocation(shaderprogram, "u_ViewMat");

	//Send data from matrices to uniform
	glUniformMatrix4fv(projmat, 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(viewmat, 1, false, glm::value_ptr(view));
}*/

/*void Light(const float time, const GLuint shaderprogram)
{
	//Get uniforms for our Light-variables.
	GLuint lightPos = glGetUniformLocation(shaderprogram, "u_LightPosition");
	GLuint lightColor = glGetUniformLocation(shaderprogram, "u_LightColor");
	GLuint lightDir = glGetUniformLocation(shaderprogram, "u_LightDirection");
	GLuint specularity = glGetUniformLocation(shaderprogram, "u_Specularity");
	GLuint cutOff = glGetUniformLocation(shaderprogram, "u_cutOff");
	GLuint outerCutOff = glGetUniformLocation(shaderprogram, "u_outerCutOff");

	glUniform1f(cutOff, glm::cos(glm::radians(12.5f)));
	glUniform1f(outerCutOff, glm::cos(glm::radians(17.5f)));


	//Send Variables to our shader
	//glUniform3f(lightPos, cos(time), 0.0f, 1 + sin(time));    //Position of a point in space. For Point lights.
	glUniform3f(lightPos, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(lightDir, cameraFront.x, cameraFront.y, cameraFront.z);                //Direction vector. For Directional Lights.

	glUniform3f(lightColor, 1.f, 1.f, 1.f);                 //RGB values
	glUniform1f(specularity, 1.f);                          //How much specular reflection we have for our object

}*/

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