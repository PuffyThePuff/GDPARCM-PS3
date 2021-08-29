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
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {

#pragma region Initialization
	// initialize glfw
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
	window = glfwCreateWindow(1024, 768, "Nathaniel Filoteo", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading

	ObjData bunnyObjData;
	LoadObjFile(&bunnyObjData, "bunny.obj");
	GLfloat bunnyOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&bunnyObjData,
		1.0f,
		bunnyOffsets
	);

#pragma endregion

#pragma region Shader Loading

	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 0.81f, 0.25f, 0.35f);

	// initialize transforms
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	glm::mat4 trans = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	// glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

	// set bg color to sky blue
	glClearColor(0.6f, 0.84f, 0.84f, 0.0f);

	// var for rotations
	float z = -3.0f;
	bool isMovingForward = true;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

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

		// Orthographic with corection for stretching, resize window to see difference with previous example
		// projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 10.0f);

		// Perspective Projection
		projection  = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 10.0f),

		// Set projection matrix in shader
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT);

		// toggle to render with GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma region Draw

		//draw bunny
		glBindVertexArray(bunnyObjData.vaoId);

		// transforms
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.05f, -0.1f, z));
		trans = glm::scale(trans, glm::vec3(0.9f, 0.9f, 0.9f));

		//send to shader
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, bunnyObjData.numFaces, GL_UNSIGNED_INT, (void*)0);

		// increment translation by deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		if (isMovingForward) {
			z += 0.8 * deltaTime;
			if(z >= -0.3f) isMovingForward = false;
		}
		else {
			z -= 0.8 * deltaTime;
			if(z <= -1.8f) isMovingForward = true;
		}
		prevTime = currentTime;

		//--- stop drawing here ---

		glfwSwapBuffers(window);
#pragma endregion
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}