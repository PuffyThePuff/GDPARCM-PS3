/*
Jose Miguel D. Bravo
Nathaniel Francis S. Filoteo
GDGRAPX S11
Virtual Museum Beta
*/

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include "skybox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
	stbi_set_flip_vertically_on_load(true);
#pragma region Initialization
	//initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Virtual Museum", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading

	ObjData earthObjData;
	LoadObjFile(&earthObjData, "Earth/Earth.obj");
	GLfloat offsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&earthObjData,
		1.0f,
		offsets
	);

	ObjData cubeObjData;
	LoadObjFile(&cubeObjData, "cube.obj");
	GLfloat cubeOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&cubeObjData,
		1.0f,
		cubeOffsets
	);

	std::vector<std::string> faces{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};
	SkyboxData skybox = LoadSkybox("Assets/Skybox", faces);

#pragma endregion

#pragma region Shader Loading

	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	glm::mat4 trans = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::mat4 inputTrans = glm::mat4(1.0f);

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glm::mat4 projectionOrtho = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

	// set bg color to sky blue
	glClearColor(0.6f, 0.84f, 0.84f, 0.0f);

	// va for inputs
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

	int state[8];
	glm::vec3 camPosition = glm::vec3(0.0f, 0.5f, 10.0f);
	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float translateFactorX = 0.0f;
	float translateFactorZ = 0.0f;
	float rotFactorPitch = 0.0f;
	float rotFactorYaw = 0.0f;


	//depth testing
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // set the depth test function

	//face culling
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // set which face to cull
	//glFrontFace(GL_CCW); // set the front face orientation

	while (!glfwWindowShouldClose(window)) {

#pragma region Viewport
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
#pragma endregion

#pragma region Projection
		// Orthographic projection but make units same as pixels. origin is lower left of window
		// projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 10.0f); // when using this scale objects really high at pixel unity size

		// Orthographic with stretching
		//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);

		//Orthographic with corection for stretching, resize window to see difference with previous example
		//projectionOrtho = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 10.0f);

		// Perspective Projection
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 10.0f);

		// Set projection matrix in shader
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View
		glm::mat4 view = glm::lookAt(camPosition, camPosition + camFront, camUp);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw
		state[0] = glfwGetKey(window, GLFW_KEY_W);
		state[1] = glfwGetKey(window, GLFW_KEY_S);
		state[2] = glfwGetKey(window, GLFW_KEY_A);
		state[3] = glfwGetKey(window, GLFW_KEY_D);

		state[4] = glfwGetKey(window, GLFW_KEY_UP);
		state[5] = glfwGetKey(window, GLFW_KEY_DOWN);
		state[6] = glfwGetKey(window, GLFW_KEY_LEFT);
		state[7] = glfwGetKey(window, GLFW_KEY_RIGHT);

		DrawSkybox(skybox, skyboxShaderProgram, view, projection);

		//draw earth
		glBindVertexArray(earthObjData.vaoId);
		glUseProgram(shaderProgram);

		// transforms

		trans = glm::mat4(1.0f);
		/*
		trans = glm::rotate(trans, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::translate(trans, glm::vec3(8.0f, 0.0f, 0.0f));
		trans = glm::scale(trans, glm::vec3(0.25f, 0.25f, 0.25f));
		trans = glm::rotate(trans, glm::radians(5 * theta), glm::vec3(0.0f, 1.0f, 0.0f));
		*/

		if (state[0] == GLFW_PRESS) {
			camPosition += 2.0f * deltaTime * camFront;
		}

		if (state[1] == GLFW_PRESS) {
			camPosition -= 2.0f * deltaTime * camFront;
		}

		if (state[2] == GLFW_PRESS) {
			camPosition -= 2.0f * deltaTime * glm::normalize(glm::cross(camUp, camFront));
		}

		if (state[3] == GLFW_PRESS) {
			camPosition += 2.0f * deltaTime * glm::normalize(glm::cross(camUp, camFront));
		}

		if (state[4] == GLFW_PRESS) {
			//rotFactorPitch += 45.0f * deltaTime;
		}

		if (state[5] == GLFW_PRESS) {
			//rotFactorPitch -= 45.0f * deltaTime;
		}

		if (state[6] == GLFW_PRESS) {
			//rotFactorYaw += 45.0f * deltaTime;
			//inputTrans = glm::mat4()
		}

		if (state[7] == GLFW_PRESS) {
			//rotFactorYaw -= 45.0f * deltaTime;
		}

		view = glm::lookAt(camPosition, camPosition + camFront, camUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//send to shader
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glActiveTexture(GL_TEXTURE0);
		GLuint earthTexture = earthObjData.textures[earthObjData.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, earthTexture);

		//draw celestial body
		glDrawElements(GL_TRIANGLES, earthObjData.numFaces, GL_UNSIGNED_INT, (void*)0);

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(cubeObjData.vaoId);
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glDrawElements(GL_TRIANGLES, cubeObjData.numFaces, GL_UNSIGNED_INT, (void*)0);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}